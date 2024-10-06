a = 0x51
b = 0x41
c = 0x21
d = 0x01
out = ''

def not_byte(x):
    return ~x & 0xFF

def f0(a, b, c, d):
    return not_byte( a & not_byte(b) | not_byte(b) & d )

def f1(a, b, c, d):
    return (a | not_byte(c)) & (b | not_byte(d))




for i in range(6):
    line = f"Counter: {i} \ta = {hex(a)}  b = {hex(b)}  c = {hex(c)}  d = {hex(d)}   \t f0: {hex(f0(a,b,c,d))}  \t  f1: {hex(f1(a,b,c,d))}"
    print(line)
    out += line + '\n'

    a += 1
    b += 2
    c += 3
    d += 4

# in the end write in file
with open('boolean_table.txt', 'w') as file:
    file.write(out)


