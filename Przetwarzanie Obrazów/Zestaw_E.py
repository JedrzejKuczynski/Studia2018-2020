import numpy as np


test_image = np.array([[0, 1, 1, 1],
					   [1, 1, 1, 1],
					   [0, 1, 0, 1],
					   [0, 1, 1, 1]])

test_kernel = np.array([[0, 1, 0],
						[0, 1, 1],
						[0, 0, 0]])

print(test_image, "\n")

def zadE1(image, kernel):
	k = len(kernel) // 2
	padded_image = np.pad(image, ((k, k), (k, k)), "constant")
	for y, x in np.ndindex(image.shape):
		result = np.sum(padded_image[y:y+kernel.shape[0], x:x+kernel.shape[1]]*kernel)
		if result > 0:
			image[y, x] = 1
		else:
			image[y, x] = 0
	return image

#print(zadE1(test_image, test_kernel), "\n")


def zadE2(image, kernel):
	k = len(kernel) // 2
	padded_image = np.pad(image, ((k, k), (k, k)), "constant")
	for y, x in np.ndindex(image.shape):
		result = np.sum(padded_image[y:y+kernel.shape[0], x:x+kernel.shape[1]]*kernel)
		if result == np.sum(kernel):
			image[y, x] = 1
		else:
			image[y, x] = 0
	return image


print(zadE2(test_image, test_kernel))
