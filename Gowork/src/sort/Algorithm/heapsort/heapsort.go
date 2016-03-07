
package heapsort

func percolateDown(values []int, index int, size int){
	
	for index*2 + 1 < size - 1{
		max := index*2 + 1
		if index*2 + 2 < size{
			if values[max] < values[max + 1]{
				max = index*2 + 2
			}
		}
		
		if values[index] > values[max]{
			break
		}else{
			values[index],values[max]=values[max],values[index]
			index = max
		}
	}
}

func HeapSort(values []int){
	size := len(values)
	
	for i := size/2 - 1; i >= 0; i-- {
		percolateDown(values,i,size)
	} 
	
	for i := size - 1; i >= 1; i-- {
		values[i],values[0]=values[0],values[i]
		size--
		percolateDown(values, 0, size)
	} 
}
