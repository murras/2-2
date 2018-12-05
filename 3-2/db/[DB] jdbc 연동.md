- `db\계정이름\product\11.2.0\dbhome_1\jdbc\lib` 폴더에 설치되어 있는 `.jar` 파일을 찾는다.
    - `ojdbc5.jar` : jdk 1.5 전용.
    - `ojdbc6.jar` : jdk 1.6 이상 전용.
- `C:\Program Files\Java\jre1.8.0_101\lib\ext`에 위 파일을 넣는다.
- 이클립스에서 프로젝트 생성.
- 모듈 추가.


- Get row count 
```java
ResultSet rs2 = st.executeQuery("select count(*) from " +fileName);
rs2.next();
int rowCount = (rs2.getInt(1));				
System.out.println(rowCount);
rs2.close();
```