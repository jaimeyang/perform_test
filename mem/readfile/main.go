package main

import (
	"flag"
	"io"
	"log"
	"os"
	"syscall"
	"time"
	"unsafe"
)

func init() {

}

const (
	// Size to align the buffer to
	AlignSize = 4096

	// Minimum block size
	BlockSize = 4096
)

var path = flag.String("p", "", "路径")
var read_num = flag.Int("n", BlockSize, "read len once")

// alignment returns alignment of the block in memory
// with reference to AlignSize
//
// Can't check alignment of a zero sized block as &block[0] is invalid
func alignment(block []byte, AlignSize int) int {
	return int(uintptr(unsafe.Pointer(&block[0])) & uintptr(AlignSize-1))
}

// AlignedBlock returns []byte of size BlockSize aligned to a multiple
// of AlignSize in memory (must be power of two)
func AlignedBlock(BlockSize int) []byte {
	block := make([]byte, BlockSize+AlignSize)
	if AlignSize == 0 {
		return block
	}
	a := alignment(block, AlignSize)
	offset := 0
	if a != 0 {
		offset = AlignSize - a
	}
	block = block[offset : offset+BlockSize]
	// Can't check alignment of a zero sized block
	if BlockSize != 0 {
		a = alignment(block, AlignSize)
		if a != 0 {
			log.Fatal("Failed to align block")
		}
	}
	return block
}

// OpenFile is a modified version of os.OpenFile which sets O_DIRECT
func OpenFile(name string, flag int, perm os.FileMode) (file *os.File, err error) {
	return os.OpenFile(name, syscall.O_DIRECT|flag, perm)
}

func main() {
	flag.Parse()
	ch := make(chan bool)

	go func() {
		for {
			block := AlignedBlock(*read_num)

			in, err := OpenFile(*path, os.O_RDONLY, 0666)
			if err != nil {
				log.Fatal("Failed to directio.OpenFile for write", err)
			}
			begin_time := time.Now().UnixNano()
			_, err = io.ReadFull(in, block)
			if err != nil {
				log.Fatal("Failed to read", err)
			}
			err = in.Close()
			if err != nil {
				log.Fatal("Failed to close reader", err)
			}
			end_time := time.Now().UnixNano()
			cost_time := float64(end_time-begin_time) / 1000000000
			log.Println("cost time is ", cost_time)
			time.Sleep(time.Second * 1)
		}
	}()
	ch <- true
	log.Println("end")
}
