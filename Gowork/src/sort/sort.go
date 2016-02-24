
package main

import	"bufio"
import	"fmt"
import	"flag"
import	"io"
import	"strconv"
import	"os"

import "Algorithm/bubblesort"


var infile *string = flag.String("i", "infile", "file contains values for sorting")
var outfile *string = flag.String("o", "outfile", "file contains values for sorted")
var algorithm *string = flag.String("a", "algorithm", "sort algorithm")

func readValues(infile string)(values []int, err error){
	file,err := os.Open(infile)
	if err != nil {
		fmt.Println("open infile failed ", infile)
		return
	}
	
	defer file.Close()
	br := bufio.NewReader(file)
	values = make([]int, 0)
	for{
		line,prefix,err1 := br.ReadLine()
		if err1 != nil{
			if err1 != io.EOF{
				err = err1
				return
			}
		}
		
		if prefix{
			fmt.Println("unexpected ", line)
			return
		}
		
		str := string(line)
		value, err1 := strconv.Atoi(str)
		
		if err1 != nil{
			err = err1
			return
		}
		
		values = append(values, value)
	}
	return
}

func WriteOutfile(values []int, outfile string)error{
	file,err :=os.Create(outfile)
	if err != nil{
		fmt.Println("create outfile failed ", infile)
		return err
	}
	defer file.Close()
	
	for _,value :=range values{
		str := strconv.Itoa(value)
		file.WriteString(str + "\n")
	} 
	return nil
}

func main(){
	flag.Parse()
	if infile != nil{
		fmt.Println("infile=", *infile, "outfile=", *outfile, "algorithm=", *algorithm);
	}
	
	values,err := readValues(*infile)
	if err == nil{
		switch *algorithm{
			case "bubblesort":
				bubblesort.Bubblesort(values)
			case "qsort":
			
			default:
		}
		WriteOutfile(values, *outfile)
	}else{
		fmt.Println(err)
	}

}





