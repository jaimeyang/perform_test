package log

import (
	"errors"
	"io"
	"log"
	"os"
	"syscall"
	"unsafe"
)

// Darwin and FreeBSD can't read or write 2GB+ files at a time,
// even on 64-bit systems.
// The same is true of socket implementations on many systems.
// See golang.org/issue/7812 and golang.org/issue/16266.
// Use 1GB instead of, say, 2GB-1, to keep subsequent reads aligned.
const maxRW = 1 << 30

type Logger struct {
	file *os.File
}

var Log *Logger

func init() {
	Log = newLogger()
}

func newLogger() *Logger {
	l := &Logger{}
	var err error
	l.file, err = os.Create("./log.log")
	if err != nil {
		log.Fatal("open log file error", err)
		return nil
	}
	return l
}

func sysWrite(fd int, p0 uintptr, l uintptr) (int, error) {
	r0, _, e1 := syscall.Syscall(syscall.SYS_WRITE, uintptr(fd), p0, l)
	n := int(r0)

	var err error
	if e1 != 0 {
		err = errors.New(e1.Error())
	}
	return n, err
}

func (l *Logger) Write(data *[]byte) (int, error) {
	var nn int
	for {
		max := len(*data)
		if max-nn > maxRW {
			max = nn + maxRW
		}
		p0 := unsafe.Pointer(&(*data)[0])
		p1 := uintptr(len(*data))
		n, err := sysWrite(int(l.file.Fd()), uintptr(p0), p1)
		if n > 0 {
			nn += n
		}
		if nn == len(*data) {
			return nn, err
		}
		if err == syscall.EAGAIN {
			continue
		}
		if err != nil {
			return nn, err
		}
		if n == 0 {
			return nn, io.ErrUnexpectedEOF
		}
	}
}
