import matplotlib.pylab as plt
import matplotlib.image as mpimg
from numpy.core.fromnumeric import resize
import tensorflow as tf

plt.figure()
plt.subplots_adjust(hspace=0.5)
plt.grid(False)
plt.suptitle("Image pre-processing")
#读取图片
image = mpimg.imread("flower_photos/daisy/5673728_71b8cb57eb.jpg")
#将图片数据转化为实数类型，[0-255] -> [0.0-1.0]
image = tf.image.convert_image_dtype(image, dtype=tf.float32)
plt.subplot(3, 2, 1)
plt.title("raw")
plt.imshow(image)
plt.axis("off")

#图像片段截取
crop_img = tf.image.resize_with_crop_or_pad(image, 100, 100)
plt.subplot(3, 2, 2)
plt.imshow(crop_img)
plt.title("crop")
plt.axis("off")

#图像大小调整
resize_img = tf.image.resize(image, [120, 100])
plt.subplot(3, 2, 3)
plt.imshow(resize_img)
plt.title("resize")
plt.axis("off")

#图像翻转
flip_img = tf.image.flip_left_right(image)
plt.subplot(3, 2, 4)
plt.imshow(flip_img)
plt.title("flip")
plt.axis("off")

#色彩调整
bright_img = tf.image.adjust_brightness(image, 0.5) # 调整明暗
plt.subplot(3, 2, 5)
plt.imshow(bright_img)
plt.title("brightness")
plt.axis("off")

saturation_img = tf.image.adjust_saturation(image, 5)   # 调整饱和度
plt.subplot(3, 2, 6)
plt.imshow(saturation_img)
plt.title("saturation")
plt.axis("off")

plt.show()