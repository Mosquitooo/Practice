
package qsort

//import "fmt"

func quicksort(values []int, left int, right int){
	if left < 0 || right < 0 || right - left < 0 {
		return
	}
	
	key := values[left]
	i,j := left,right
	for i < j {
		for i<j && values[j] > key{
			j--
		}
		values[i] = values[j]
		
		for i<j && values[i] < key{
			i++
		}
		values[j] = values[i]
	}
	values[i] = key

	//quicksort(values, left, i - 1)
	//quicksort(values, i+1, right)
}

func QuickSort(values []int){
	quicksort(values, 0, len(values) - 1)
}