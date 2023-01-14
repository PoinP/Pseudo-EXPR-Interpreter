@page examples Examples
@tableofcontents

@section tobinary Decimal to Binary

### Iterative
```
read decimal

binary = 0
step = 1

while decimal > 0 do
    lastdigit = decimal % 2
    binary = lastdigit * step + binary
    step = step * 10
    decimal = decimal / 2
done

print binary
```

### Recursive
```
TOBINARY[x] = if x > 0 then (x % 2 + TOBINARY[x / 2] * 10) else 0

read decimal

print TOBINARY[decimal]
```

@section digitssum Sum of the Digits of a Number

```
read n
sum = 0

while n > 0 do
    sum = sum + n % 10
    n = n / 10
done

print sum
```

@section fibbonacci Fibonacci Calculation
### Iterative
```
read n

fib0 = 1
fib1 = 1

i = 2
while i < n do
    tempfib = fib1
    fib1 = fib0 + fib1
    fib0 = tempfib
    i = i + 1
done

fib1 = n == 0 ? 0 : fib1

print fib1
```

### Recursive
```
FIB[x] = if x == 0 then 0 else if x < 2 then 1 else FIB[x - 1] + FIB[x - 2]

read fib

print FIB[fib]
```