import numpy as np
import time
import PIL.Image as Image
import matplotlib.pylab as plt
import tensorflow as tf
import tensorflow_hub as hub
import datetime

# 使用inception_v3提取特征
inception_v3 = "https://tfhub.dev/google/tf2-preview/inception_v3/feature_vector/4"
feature_extractor_model = inception_v3

#TensorFlow 花朵模型
# data_root = tf.keras.utils.get_file(
#   'flower_photos',
#   'https://storage.googleapis.com/download.tensorflow.org/example_images/flower_photos.tgz',
#    untar=True)

#首先，使用磁盘外的图像数据将此数据加载到模型中tf.keras.utils.image_dataset_from_directory，这将生成一个tf.data.Dataset：
batch_size = 32
img_height = 224
img_width = 224

#TensorFlow 花朵模型
train_ds = tf.keras.utils.image_dataset_from_directory(
  "flower_photos/",
  validation_split=0.2,
  subset="training",
  seed=123,
  image_size=(img_height, img_width),
  batch_size=batch_size
)

val_ds = tf.keras.preprocessing.image_dataset_from_directory(
  "flower_photos/",
  validation_split=0.2,
  subset="validation",
  seed=123,
  image_size=(img_height, img_width),
  batch_size=batch_size
)

# 花数据有5个类
class_names = np.array(train_ds.class_names)
# print(class_names)

#预处理数据
normalization_layer = tf.keras.layers.Rescaling(1./255)
train_ds = train_ds.map(lambda x, y: (normalization_layer(x), y)) # Where x—images, y—labels.
val_ds = val_ds.map(lambda x, y: (normalization_layer(x), y)) # Where x—images, y—labels.

# 通过使用带缓冲的预取来完成输入管道Dataset.prefetch，这样您就可以从磁盘生成数据而不会出现 I/O 阻塞问题
AUTOTUNE = tf.data.AUTOTUNE
train_ds = train_ds.cache().prefetch(buffer_size=AUTOTUNE)
val_ds = val_ds.cache().prefetch(buffer_size=AUTOTUNE)

for image_batch, labels_batch in train_ds: break

#通过将预训练模型包装为 Keras 层来创建特征提取器hub.KerasLayer。使用trainable=False参数冻结变量，以便训练只修改新的分类器层：
feature_extractor_layer = hub.KerasLayer(
    feature_extractor_model,
    input_shape=(224, 224, 3),
    trainable=False)
# 特征提取器为每个图像返回一个2048向量（在本例中，图像批量大小保持为 32）：
feature_batch = feature_extractor_layer(image_batch)

#为了完成模型，将特征提取器层包裹在一个tf.keras.Sequential模型中，并添加一个全连接层进行分类
num_classes = len(class_names)

#附上classification head
model = tf.keras.Sequential([
  feature_extractor_layer,
  tf.keras.layers.Dense(num_classes) #classification head
])

model.summary()
# 训练模型
# 使用Model.compile训练
model.compile(
  optimizer=tf.keras.optimizers.Adam(),
  loss=tf.keras.losses.SparseCategoricalCrossentropy(from_logits=True),
  metrics=['acc'])
# 添加一个tf.keras.callbacks.TensorBoard回调来创建和存储日志：
log_dir = "logs/fit/" + datetime.datetime.now().strftime("%Y%m%d-%H%M%S")
tensorboard_callback = tf.keras.callbacks.TensorBoard(
    log_dir=log_dir,
    histogram_freq=1) # Enable histogram computation for every epoch.
# 为了防止过拟合，设置early stopping
early_stopping = callback = tf.keras.callbacks.EarlyStopping(monitor='loss', patience=3)

# EPOCHS
NUM_EPOCHS = 10

history = model.fit(train_ds,
                    validation_data=val_ds,
                    epochs=NUM_EPOCHS,
                    callbacks=[tensorboard_callback, early_stopping])