## 네트워크 프로그래밍 과제 1
- 랜덤으로 10000줄의 무작위 데이터를 만들어야한다.
- `[줄번호] 데이터` 형식으로 만든다. 데이터 크기는 50바이트로 정한다.
- `IncludeNumber=True`이면 숫자 포함한 데이터를 생성하고 False면 알파벳만 포함한다.

```python
import random

ASCII_number_start = 48
ASCII_number_end = 57
ASCII_AlphabetUpper_start = 65
ASCII_AlphabetUpper_end = 90
ASCII_AlphabetLower_start = 97
ASCII_AlphabetLower_end = 122
```

- 파일 생성 갯수
- 확장자
- Line 수
- 한 Line당 길이
- 데이터에 숫자 포함 여부


```python
HowManyFile = 3
Extension = '.txt'
HowManyLine = 10000
OneLineLenghth = 50
IncludeNumber = False
```


```python
if IncludeNumber :
    randArg = 3
else :
    randArg = 2
```


```python
fileNames = []
for i in range(HowManyFile) :
    fileNames.append("input_random_"+str(i+1) + Extension)
```


```python
for fileName in fileNames :
    with  open(fileName, 'w') as file :
        for i in range(HowManyLine):
            inputLine = "[" + str(i+1) + "] "
            for line in range(OneLineLenghth) : 
                case = random.randint(1, randArg)
                if case == 1:
                    inputLine += chr(random.randint(ASCII_AlphabetUpper_start, ASCII_AlphabetUpper_end))
                if case == 2:                    
                    inputLine += chr(random.randint(ASCII_AlphabetLower_start, ASCII_AlphabetLower_end))
                if case == 3:
                    inputLine += chr(random.randint(ASCII_number_start, ASCII_number_end))
            inputLine += '\n'
            file.write(inputLine)
```
