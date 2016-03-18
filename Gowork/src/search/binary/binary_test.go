
package binary

import "testing"

func TestQuickSort1(t *testing.T) {
	values := []int{1,2,3,4,5}
	if Binary(values,1) != 0{
		t.Error("Binary() failed. Got", values, "Expected 0")
	}
}

func TestQuickSort2(t *testing.T) {
	values := []int{1,2,3,4,5}
	if Binary(values,2) != 1{
		t.Error("Binary() failed. Got", values, "Expected 1")
	}
}

func TestQuickSort3(t *testing.T) {
	values := []int{1,2,3,4,5}
	if Binary(values,3) != 2{
		t.Error("Binary() failed. Got", values, "Expected 2")
	}
}

func TestQuickSort4(t *testing.T) {
	values := []int{1,2,3,4,5}
	if Binary(values,4) != 3{
		t.Error("Binary() failed. Got", values, "Expected 3 ")
	}
}

func TestQuickSort5(t *testing.T) {
	values := []int{1,2,3,4,5}
	if Binary(values,5) != 4{
		t.Error("Binary() failed. Got", values, "Expected 4")
	}
}

