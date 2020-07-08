package main

import (
	"math/rand"
	"runtime"
	"time"
	"write/log"
)

func init() {
	runtime.GOMAXPROCS(1)
}

func getRandomString() []byte {
	str := "0123456789abcdefghijklmnopqrstuvwxyz"
	bytes := []byte(str)
	result := []byte{}
	r := rand.New(rand.NewSource(time.Now().UnixNano()))
	size := 300 * 1024 * 1024
	for i := 0; i < size; i++ {
		result = append(result, bytes[r.Intn(len(bytes))])
	}
	return result
}

func main() {
	ch := make(chan bool)
	data := getRandomString()
	go func() {
		for {
			log.Log.Write(&data)
			time.Sleep(time.Millisecond * 100)
		}
	}()
	isEnd := <-ch
	println("isENd", isEnd)
}
