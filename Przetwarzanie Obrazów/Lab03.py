import numpy as np

def zadA1(image):
	return 255 - image

def testA1():
	result = zadA1(np.array([[0,1,255]], np.uint8))
	expected = [[255,254,0]]
 
	np.testing.assert_array_equal(result, expected)

def zadA2(image, thr):
	image[image > thr] = 255
	image[image <= thr] = 0
	return image

def testA2():
	#normalny
	result = zadA2(np.array([[128, 255, 43, 56],
							[41, 88, 128, 36]], np.uint8), 42)
	expected = [[255, 255, 255, 255],
				[0, 255, 255, 0]]
	np.testing.assert_array_equal(result, expected)
	
	#dla 255 thr=255
	result = zadA2(np.array([[255, 255, 255, 255],
							 [255, 255, 255, 255]], np.uint8), 255)
	expected = [[0, 0, 0, 0],
				[0, 0, 0, 0]]
	np.testing.assert_array_equal(result, expected)

def zadA3(image, gamma):
	image = image.astype(float)
	image /= 255
	image = image**gamma
	return np.rint(image*255)

def testA3():
	#normalny
	result = zadA3(np.array([[255, 128, 43, 0],
							 [128, 87, 20, 255]], np.uint8), 2)
	expected = [[255, 64, 7, 0],
				[64, 30, 2, 255]]
	np.testing.assert_array_equal(result, expected)
	
	#test Kasi
	result = zadA3(np.array([[1, 0, 255],
							 [0, 4, 1],
							 [5, 0, 1]], np.uint8), 0.5)
	expected = [[16, 0, 255],
				[0, 32, 16],
				[36, 0, 16]]
	np.testing.assert_array_equal(result, expected)


testA3()
