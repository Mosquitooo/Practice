
// qsort_test.go
package qsort

import "testing"

func TestMergerSort1(t *testing.T) {
	values := []int{5, 4, 3, 2, 1}
	MergerSort(values)
	if values[0] != 1 || values[1] != 2 || values[2] != 3 || values[3] != 4 ||
	values[4] !=5 {
		t.Error("MergerSort() failed. Got", values, "Expected 1 2 3 4 5")
	}
}
func TestMergerSort2(t *testing.T) {
	values := []int{5, 5, 3, 2, 1}
	MergerSort(values)
	if values[0] != 1 || values[1] != 2 || values[2] != 3 || values[3] != 5 ||
	values[4] !=5 {
		t.Error("MergerSort() failed. Got", values, "Expected 1 2 3 5 5")
	}
}
func TestMergerSort3(t *testing.T) {
	values := []int{5}
	MergerSort(values)
	if values[0] != 5 {
		t.Error("MergerSort() failed. Got", values, "Expected 5")
	}
}