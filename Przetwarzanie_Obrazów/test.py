import numpy as np

def zadA1(image):
    return 255 - image
 
def testA1():
    result = zadA1(np.array([[0,1,255]], np.uint8))
    expected = [[255,254,0]]
 
    np.testing.assert_array_equal(result, expected)
    
    # lub gdy mogą wystąpić błędy zaokrągleń
    # np.testing.assert_allclose(result, expected)
    
    # tutaj inne testy
    # ...
 
if __name__ == "__main__":
    testA1()
