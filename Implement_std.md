### Computing Assignment 2 Implement Standard
version 0.7
Apr. 22nd 2022, Definition of Query
Apr. 23rd 2022, Definition of Data Storage
Apr. 23rd 2022, Priority Rules Definition Updated
Apr. 24th 2022, RECORD.ID is now an index key
***
#### (I) Overview
1. 图形化用户接口 GUI
-   图形化用户接口， 显示信息并接收指令。
-   Graphic User Interface, display information and send instructions to other parts
2. 数据控制 Access Control
-   提供一个查询命令接口，将查询结果返回。
-   Support an interface that receive and process a query command, and return the query results
-   提供一个从文件添加/改变数据的接口。
-   Support an interface that add / change the data from a data file
-   提供一个直接添加/改变数据的接口。
-   Support an interface that add / change the data directly
3. 模拟器 Simulator
-   提供一个触发接口，返回该时刻的状态，并推演下一时刻
-   提供一个从文件添加/改变数据的接口
-   提供一个设置模拟条件的接口
4. 数据存储 Data Storage
-   提供一个通过键值查询值的接口
-   提供一个增加值的接口
-   提供一个删除值的接口
-   提供一个查询区间的接口
***
#### (II) GUI Standard
***
Already Implemented.
#### (III) Access Control Standard 
1. 查询语句
除特殊说明外，查询语句不区分大小写
    1. 关键字：FROM, SELECT, WHERE, ORDER, AND, OR, NOT, ALL
    2. 支持的符号： (, ), <, >, ==, <=, =>
    3. 关键字定义：
        1. FROM:  选择查询目标。
        2. SELECT: 选择需要返回的项的元素。
        3. WHERE:   设置查询目标的条件。 定义WHERE后至少存在一组半角括弧()。所有括弧均应当匹配。
        4. ORDER:  设置排序的关键字。 定义ORDER后存在一组半角括弧(),内部为(key,order)。其中key为关键字，order为数字0或1， 代表升序或降序。
        5. AND:    逻辑与
        6. OR:     逻辑或
        7. NOT:    逻辑非
        8. ALL:    所有可用的元素
    4.  查询关键字类型：
        1. 整型，无符号长整型：表示方式为数字
        2. 字符串： 表示时外加""
        3. 日期时间型："yyyy/mm/dd"，"yyyy/mm/dd/hh/nn/ss";
        4. 布尔型：该类型不支持查询
    5.  查询目标及目标元素定义：
        查询目标共两类，分别为RECORD与PERSON
        1. RECORD类中元素包括：（带**的为主键，带*的为可索引的元素）
            1. UID**: 唯一标签，无符号长整型
            2. NAME: 姓名，字符串
            3. ID*:  证件号码，字符串
            4. BIRTH: 生日，日期时间型 
            5. PROF*: 职业，整型
            6. STATUS*: 状态， 整型 (0:Registed, 1:Queuing, 2:Assigned, 3:Treated, 4:Withdrawn, 5:Not Registed)
            7. RISKGP*: 危机等级， 整型
            8. AGEGP*:   年龄组， 整型
            9. REGAT:   注册点编号， 整型
            10. REGTIME*: 注册时间， 日期时间型
            11. QUETIME*: 进入等待队列时间，日期时间型
            12. TREATTP*: 治疗种类， 整型
            13. TREATAT: 医院编号， 整型
            14. ASSTIME*: 预约时间， 日期时间型
            15. TRETIME*: 治疗时间， 日期时间型
            16. EMERGENCY: 是否紧急， 布尔型
            17. DEADLINE: 紧急救治截止时间， 日期时间型
        2. PERSON类中元素包括：（带**的为主键，带*的为可索引的元素）
            1. NAME: 姓名，字符串
            2. ID**:  证件号码，字符串
            3. BIRTH* : 生日，日期型 
            4. PROF*: 职业，整型
    6. 语句定义：
        1. 每一个查询语句为一个字符串。必须出现 FROM, SELECT关键字
        2. FROM 后，必须为RECORD或PERSON，用若干空格隔开。
        3. SELECT 后，必须为一组括弧。括弧与SELECT用若干空格隔开。括弧内含有元素名称或者ALL。若存在两个及以上元素，用半角逗号(,)隔开。若存在ALL，无视其他元素。
        4. 若 WHERE 关键字不出现，则返回所有的类中内容。
        5. 若 WHERE 关键字出现，则其后必须存在一组半角括弧，括弧内包含筛选条件。
        6. 若 ORDER 关键字不出现，返回值默认以主键升序排序。
        7. 若 ORDER 关键字出现，则其后必须存在一组半角括弧，括弧内包含排序关键字，以及排序顺序。
        8. 筛选条件由一个或多个元素筛选条件通过AND, OR, NOT关键字以及括弧连接而成，优先级由大到小为括弧，NOT, AND，OR。 每一个元素筛选条件均应当被括弧包括。
        9. 元素筛选条件的格式为如下格式之一：
            KEY 为元素名称， CONST为条件常量
            1. KEY < CONST
            2. KEY <= CONST
            3. KEY > CONST
            4. KEY >= CONST
            5. KEY == CONST
            6. CONST1 < KEY < CONST2
            7. CONST1 <= KEY < CONST2
            8. CONST1 < KEY <= CONST2
            9. CONST1 <= KEY <= CONST2
            注意，为方便起见，约定上述格式必须严格匹配。例如表达式CONST < KEY是非法的
        10. 排序条件由 元素名称以及一个数字构成。 0代表升序， 1 代表降序。
    7. 语句示例：
        从RECORD中提取在1970年1月1日0时0分0秒后注册的病人的注册信息，并且该病人的职业在3到5之间。返回结果以UID为关键字升序排序。
        `FROM RECORD SELECT (ALL) WHERE ((REGTIME > "1970/01/01/00/00/00")AND(3 <= PROF <= 5)) ORDER (UID,0)`
        `FROM RECORD SELECT (ALL) WHERE (STATUS == 5) ORDER (UID,0)`
2. 查询实现接口定义
    接收字符串，返回STL向量型指针。
    接口定义：
    `vector<vector<string>*>* Query (string s);`
    返回的指针指向一个`vector`实例，该实例的元素为`vector<string>*`，即一个指向`vector<string>`的指针。
    为方便说明，现在定义：`vector<vector<string>*>* v1`。
    `v1 -> at(0)`返回的指针应当指向一个`vector<string>`,其中包含了返回的元素名称信息，如"UID"等。
    可以理解为，`v1`中的首项，包含了这个返回的数据表的表头，即每一列的元素的名称。
    对于`v1 -> at(i)`(i > 0) 返回的指针应当指向一个 `vector<string>`,其中包含了排序为i的项的，指定返回的元素的字符串表达。对于整型，无符号长整型，将其转换为字符串表达。对于日期时间型，除了BIRTH元素使用DateTime.ToShortString()方法，其余使用DateTime.ToString()方法。对于布尔型，True对应的字符串表达为"Y",False对应的字符串表达为"N".
    可以理解为，`v1`中的除了首项外的任何一项，代表返回数据表中的一行。
    若语句无效，或无任何符合条件的项，则返回`nullptr`.
3. 增加，修改数据接口定义：
    1. 从文件添加数据：
    `int Put(string filename)`
    该接口应当从一个原始记录表文件中读取Record。并在DataStorage中建立对应的索引。注意应当对UID，Status等信息进行初始化
    若成功则返回0，若文件不存在，读取错误返回1，其他错误返回2
    2. 从文件更新数据：
    `int UpdateData(string filename)`
    该接口应当从一个更新记录表文件中读取Record。并在DataStorage中更新对应的索引。注意不要重复添加没有更新的索引。
    忽略不存在的需要更新的记录。
    若成功则返回0，若文件不存在，读取错误返回1，其他错误返回2
    3. 直接添加数据：
    `int Put(Record* r)`
    该接口应当对Record* r在DataStorage中建立对应的索引。
    若成功则返回0，否则返回1.
    4. 直接更新数据：
    `int UpdateData(Record* r)`
    该接口应当对UID为r->uid的记录进行更新。注意不要重复添加没有更新的索引。
    若成功则返回0，若记录不存在返回1，其他错误返回2
    注：①AGEGP不能直接改，应该改Birthday。
    ②所有带时分秒的日期格式统一为："yyyy/mm/dd-hh:mm:ss"（不含引号）。
    5. 更新信息暴露：
    `std::list<Record*> newrecords`
    `std::list<Record*> updatedrecords`
4. 交互文件格式：
    1. 原始记录表文件：
        首行：date,time,name,id,addr,cord,phone,Wechat,email,prof,Birthday,Risk,TreatType
        项目行：yyyy/mm/dd,hh/mm/ss,name,id,addr,a|b,phonenumber,Wechat,email,prof,yyyy/mm/dd,Risk,TreatType
    2. 更新记录表文件：
        首行：uid,(需要更新的元素名称)
        项目行：uid,Item
***
#### (IV) Simulator Standard 
1. 治疗比较规则

    规则1：Priority_Rule1,参数为(const Record* a,const Record* b), a 先于 b 则返回true

    Rule 1: Priority_Rule1. The parameter is (const record * a, const record * b);

        If a have higher priority than b, it returns true, otherwise return false
        1. 首先比较两者是否有加急信件，有加急信件的更高;
            如果两者都有加急信件,那么ddl先的更高
        1. First, find that whether the two have "isEmergency", which means a Emergency letter.
            the one have isEmergy is higher; 
            if both have "isEmergency", the deadline that is eariler have higher priority.
        2. 然后，如果两者都没有加急信件，那么比较risk_gp，
            如果一方为3，一方不为3，那么不为3的更高。
        2. Then, if neither have a Emergency letter, then the risk_gp is used to compare,
            if one have 3 of risk_gp and the other is not 3, 
            the one that have risk_gp that is 3 have a higher priority.
        3. 然后，如果两者的risk_gp都为3或者都不为3，那么比较prof,其中prof更大的为更高
        3. Then, if both have risk_gp that are not 3 or that are both 3, compare prof.
            the higher prof have a higher priority.
        4. 然后，如果两者的prof一样，那么比较age_gp, 其中age_gp更大的为更高。
        4. Then, if both prof is the same, compare age_gp;
            where age_gp higher, the priority is higher.
        5. 最后比较considertime,两者之间considertime更早的为更高的优先级。
            比较方法是如果b比a晚那么返回true, 否则返回false.
        5. Finally, compare the considertime, 
            and the earlier considertime is the higher priority.
            The comparison method is to return true if b is later than a, otherwise false

    规则2：Priority_Rule2,参数为(const Record* a,const Record* b), a 先于 b 则返回true

    Rule 2: Priority_Rule2. The parameter is (const record * a, const record * b);

        If a have higher priority than b, it returns true, otherwise return false
        1. 首先比较两者是否有加急信件，有加急信件的更高;
            如果两者都有加急信件,那么ddl先的更高
        1. First, find that whether the two have "isEmergency", which means a Emergency letter.
            the one have isEmergy is higher; 
            if both have "isEmergency", the deadline that is eariler have higher priority.
        2. 然后，如果两者都没有加急信件，那么比较risk_gp，
            其中risk_gp更小的更高。
        2. Then, if neither have a Emergency letter, then the risk_gp is used to compare,
            the one have smaller risk_gp is higher.
        3. 然后，如果两者的risk_gp一样，那么比较prof,其中prof更小的为更高
        3. Then, if both have same risk_gp, compare prof.
            the SMALLER prof have a higher priority.
        4. 然后，如果两者的prof一样，那么比较age_gp, 其中age_gp更小的为更高。
        4. Then, if both prof is the same, compare age_gp;
            where age_gp SMALLER, the priority is higher.
        5. 最后比较considertime,两者之间considertime更早的为更高的优先级。
            比较方法是如果b比a晚那么返回true, 否则返回false.
        5. Finally, compare the considertime, 
            and the earlier considertime is the higher priority.
            The comparison method is to return true if b is later than a, otherwise false
    规则3：Priority_Rule3,参数为(const Record* a,const Record* b), a 先于 b 则返回true
    
    Rule 3: Priority_Rule3. The parameter is (const record * a, const record * b);

        If a have higher priority than b, it returns true, otherwise return false
        1. 首先比较age_gp, 其中age_gp更大的为更高。
        1. First, compare age_gp;
            where age_gp larger, the priority is higher.
        2. 然后如果age_gp一样，比较risk_gp，risk_gp更小的更高。
        2. Then if they have same age_gp, compare risk_gp, 
            the one have smaller risk_gp have the higher priority.
        3. 然后如果risk_gp一样，比较considertime, 其中早的更高。
        3. Then if they have same risk_gp, compare considertime,
            the one have eariler considertime have higher priority.
        4. 然后比较两者是否有加急信件，有加急信件的更高;
            如果两者都有加急信件,那么ddl先的更高
        4. Then find that whether the two have "isEmergency", which means a Emergency letter.
            the one have isEmergy is higher; 
            if both have "isEmergency", the deadline that is eariler have higher priority.
        5. 最后，如果a的prof比b的大，那么认为a比b高，否则认为b比a高
        5. Finally, if a have larger prof than b, then consider that a have higher Priority than b,
            otherwise, consider that b have higher Priority than a.
2. 辅助结构定义

```
```

***
#### (V) Data Storage Standard
1. B+树 (BPlus):标准B+树。实现如下接口：
```
    /*key: an instant of key; item: corresponding item pointer;
    If the key is not in the tree, insert it. Otherwise, update its T* with the new one */
    void Insert(K key, T* item);
    /*key: an instant of key; 
    Return the corresponding value. 
    Return nullptr if not found.*/              
    T* Find(K key); 
    /*key_min: define the lower bound of the range; key_max: define the upper bound of the range. min_inclusive: decide if the lower bound is inclusive. max_inclusive: decide if the upper bound is inclusive. 
     Return a pointer to vector<T*> (created by "new") containing all the items whose keys are within the range.
     Return nullptr if there is no such item.*/
   vector<T*>* Range (K key_min, K key_max, bool min_inclusive, bool max_inclusive); 
```
2. 重关键字B+树 (BPlus_multi)：允许存在关键字相同的项。实现如下接口：
```
    /*key: an instant of key; item: corresponding item pointer; 
    Insert a new node */
    void Insert(K key, T* item);
    /*key: an instant of key; 
    Return the corresponding value(s) in a vector<T*> (created by "new"). 
    Return nullptr if not found.*/              
    vector<T*>* Find(K key); 
    /*key_min: define the lower bound of the range; key_max: define the upper bound of the range. min_inclusive: decide if the lower bound is inclusive. max_inclusive: decide if the upper bound is inclusive. 
     Return a pointer to vector<T*> (created by "new") containing all the items whose keys are within the range.
     Return nullptr if there is no such item.
     Invalid nodes are deleted in this process.
     */
    vector<T*>* Range (K key_min, K key_max,bool min_inclusive, bool max_inclusive); 
```
3. 静态数据存储接口：(DataStorage)
```
    static BPlus<unsigned long, Record> recstore_uid;   //Record主键(UID)BPlus树
    static BPlus_multi<int, Record> recstore_prof;      //Record次要键(PROF)BPlus树
    static BPlus_multi<int, Record> recstore_status;    //Record次要键(STATUS)BPlus树
    static BPlus_multi<int, Record> recstore_riskgp;    //Record次要键(RISKGP)BPlus树
    static BPlus_multi<int, Record> recstore_agegp;     //Record次要键(AGEGP)BPlus树
    static BPlus_multi<DateTime, Record> recstore_regtime;  //Record次要键(REGTIME)BPlus树
    static BPlus_multi<DateTime, Record> recstore_quetime;  //Record次要键(QUETIME)BPlus树
    static BPlus_multi<int, Record> recstore_treattp;   //Record次要键(TREATTP)BPlus树
    static BPlus_multi<DateTime, Record> recstore_asstime;  //Record次要键(ASSTIME)BPlus树
    static BPlus_multi<DateTime, Record> recstore_tretime;  //Record次要键(TRETIME)BPlus树
    static BPlus<string, Record> personstore_ID;        //Person主键(ID)BPlus树
    static BPlus_multi<DateTime, Record> personstore_birth; //Person次要键(BIRTH)BPlus树
    static BPlus_multi<int, Record> personstore_prof;   //Person次要键(PROF)BPlus树
```
4. 静态辅助常量：(用于Range()搜索)
```
    const static string maxstring;
    const static string minstring;
    const static DateTime maxDateTime;
    const static DateTime minDateTime;
    const static unsigned long maxULong;
    const static unsigned long minULong; 
```

接口使用示例: 返回UID为12的Record的记录`DataStorage::recstore_uid.Find(12);`
***