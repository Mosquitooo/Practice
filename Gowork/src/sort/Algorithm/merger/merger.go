
package merger

func merger(left,right []int)(result []int){
	l,r:=0,0
	for l < len(left) && r < len(right){
		if(left[l] <= right[r]){
			result = append(result,left[l])
			l++
		}else{
			result = append(result,right[r])
			r++
		}
	}
	
	result = append(result, left[l:]...)   
	result = append(result, right[r:]...)
	return
}

func MergerSort(values []int)[]int{
	length := len(values)   
	if length <= 1 {
		return values 
	}   
	num := length / 2
	left := MergerSort(values[:num])   
	right := MergerSort(values[num:])   
	return merger(left, right)
}
