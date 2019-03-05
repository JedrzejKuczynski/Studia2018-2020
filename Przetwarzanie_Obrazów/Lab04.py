import numpy as np


def zadC1(image):
	return np.histogram(image, bins=256, range=(0, 256), density=True)


def testC1():  # pozniej
	result = zadC1(np.array([[255, 255, 0],
							 [128, 128, 0],
							 [255, 0, 128]]))
	#expected = np.array([])


def zadC2(image):
	img_min = np.min(image)
	img_max = np.max(image)
	return np.rint(((image - img_min)/(img_max-img_min))*255)
	


print(zadC2(np.array([[1, 0, 0],
							 [0, 4, 1],
							 [5, 0, 1]])))
