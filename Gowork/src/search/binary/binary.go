
package binary

func Binary(values []int, key int)(index int){
	left := 0;
	right:= len(values) - 1;
	for left <= right{
		middle := (left + right)/2
		if key == values[middle] {
			return middle
		}else if key > values[middle]{
			left = middle + 1
		}else{
			right = middle - 1
		}
	}
	return -1
}