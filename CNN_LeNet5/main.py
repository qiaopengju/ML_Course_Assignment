# TensorFlow and tf.keras
import tensorflow as tf
from tensorflow import keras

# Helper libraries
import numpy as np
import matplotlib.pyplot as plt
from datetime import datetime

#导入mnist数据集
mnist = keras.datasets.mnist
(train_images, train_labels), (test_images, test_labels) = mnist.load_data()
#将数据转化为[0, 1]
train_images = train_images / 255.0
test_images = test_images / 255.0
#设置图像通道数量，mnist是灰度图像，故通道为1; 如果是RGB图形，通道为3
train_images = train_images.reshape((train_images.shape[0], train_images.shape[1], train_images.shape[2], 1))
test_images = test_images.reshape((test_images.shape[0], test_images.shape[1], test_images.shape[2], 1))

#设置NeLet-5模型层次
model = keras.Sequential([
    keras.layers.Conv2D(6, kernel_size = 5, strides = 1, activation = "relu"),  #Convolution Layer 1
    keras.layers.MaxPool2D(pool_size = 2, strides = 2),                         #Pooling Layer 2
    keras.layers.Conv2D(16, kernel_size = 5, strides = 1, activation = "relu"), #Convolution Layer 3
    keras.layers.MaxPool2D(pool_size = 2, strides = 2),                         #Pooling Layer 4
    keras.layers.Flatten(input_shape = (28, 28)),                               #Flatten, 与之后一层全连接
    keras.layers.Dense(120, activation="relu"),                                 #Fully connected layer 5
    keras.layers.Dense(84, activation="relu"),                                  #Fully connected layer 6
    keras.layers.Dense(10)                                                      #输出结果
])

#输出模型结构
#model.build(input_shape=(None, 28, 28, 1))
#model.summary()

#Define the Keras TensorBoard callback.
logdir="logs/" + datetime.now().strftime("%Y%m%d-%H%M%S")
tensorboard_callback = keras.callbacks.TensorBoard(log_dir=logdir)

#模型装配
model.compile(optimizer='adam',
              loss=tf.keras.losses.SparseCategoricalCrossentropy(from_logits=True),
              metrics=['accuracy'])

#模型训练
model.fit(train_images, train_labels, epochs=5, callbacks=[tensorboard_callback])

#模型测试
test_loss, test_acc = model.evaluate(test_images,  test_labels, verbose=2)

#输出测试结果
print('\nTest accuracy:', test_acc)