# S-AES接口文档

#### 类 `AES`

##### 构造函数
- `AES()`
   - 描述：创建一个AES对象。
   - 参数：无

##### 方法
- `QString decToBin(int num, int width)`
   - 描述：将十进制数转换为指定宽度的二进制字符串。
   - 参数：
     - `num`：要转换的十进制数。
     - `width`：所需的二进制字符串的宽度。
   - 返回值：转换后的二进制字符串。

- `int binToDec(const QString& binStr)`
   - 描述：将二进制字符串转换为十进制数。
   - 参数：
     - `binStr`：要转换的二进制字符串。
   - 返回值：转换后的十进制数。

- `QString substituteBytes(const QString& state, bool inverse)`
   - 描述：进行字节代换操作。
   - 参数：
     - `state`：要进行字节代换的状态字符串。
     - `inverse`：布尔值，指示是否进行逆向字节代换。
   - 返回值：进行字节代换后的新状态字符串。

- `QString shiftRows(const QString& state, bool inverse)`
   - 描述：进行行位移操作。
   - 参数：
     - `state`：要进行行位移的状态字符串。
     - `inverse`：布尔值，指示是否进行逆向行位移。
   - 返回值：进行行位移后的新状态字符串。

- `QString mixColumns(const QString& state, bool inverse)`
   - 描述：进行列混合操作。
   - 参数：
     - `state`：要进行列混合的状态字符串。
     - `inverse`：布尔值，指示是否进行逆向列混合。
   - 返回值：进行列混合后的新状态字符串。

- `QString addRoundKey(const QString& state, const QString& key)`
   - 描述：进行轮密钥加操作。
   - 参数：
     - `state`：要进行轮密钥加的状态字符串。
     - `key`：要添加的轮密钥。
   - 返回值：进行轮密钥加后的新状态字符串。

- `QString expendkey(const QString& key, int round)`
   - 描述：扩展密钥。
   - 参数：
     - `key`：初始密钥。
     - `round`：当前轮数。
   - 返回值：扩展后的轮密钥字符串。

- `QString encryptBlock(const QString& plaintext, const QString& key)`
   - 描述：对单个数据块进行AES加密。
   - 参数：
     - `plaintext`：要加密的明文数据块。
     - `key`：加密密钥。
   - 返回值：加密后的密文数据块。

- `QString decryptBlock(const QString& ciphertext, const QString& key)`
   - 描述：对单个数据块进行AES解密。
   - 参数：
     - `ciphertext`：要解密的密文数据块。
     - `key`：解密密钥。
   - 返回值：解密后的明文数据块。

- `QString encrypt(const QString& plaintext, const QString& key)`
   - 描述：对整个明文进行AES加密。
   - 参数：
     - `plaintext`：要加密的明文字符串。
     - `key`：加密密钥。
   - 返回值：加密后的密文字符串。

- `QString decrypt(const QString& ciphertext, const QString& key)`
   - 描述：对整个密文进行AES解密。
   - 参数：
     - `ciphertext`：要解密的密文字符串。
     - `key`：解密密钥。
   - 返回值：解密后的明文字符串。

#### 类 `CrackWidget`
- `CrackWidget(QWidget *parent = nullptr)`
  - 描述：破解界面的窗口部件构造函数。
  - 参数：
    - `parent`：可选的父级窗口部件，默认为空。
- `void crack()`
  - 描述：执行破解操作的槽函数。
  - 参数：无。

##### 属性
- `QPlainTextEdit* m_plainTextEdit`
  - 描述：明文的文本编辑框。
- `QPlainTextEdit* m_cipherTextEdit`
  - 描述：密文的文本编辑框。
- `QLineEdit* m_keyEdit`
  - 描述：破解后的密钥的单行文本编辑框。
- `QLineEdit* m_timeLabel`
  - 描述：破解所用时间的单行文本编辑框。

##### 信号
无

##### 公共槽函数
- `void crack()`
  - 描述：执行破解操作的槽函数。

#### 类 `decryptCBC`
- `decryptCBC(QWidget *parent = nullptr)`
  - 描述：解密界面的窗口部件构造函数。
  - 参数：
    - `parent`：可选的父级窗口部件，默认为空。
- `void decrypt()`
  - 描述：执行解密操作的槽函数。
  - 参数：无。

##### 属性
- `QPlainTextEdit* m_cipherTextEdit`
  - 描述：密文的文本编辑框。
- `QLineEdit* m_keyEdit`
  - 描述：密钥的单行文本编辑框。
- `QLineEdit* m_ivEdit`
  - 描述：本轮随机数的单行文本编辑框。
- `QLabel* m_decryptedLabel`
  - 描述：解密结果的标签。

##### 信号
无

##### 公共槽函数
- `void decrypt()`
  - 描述：执行解密操作的槽函数。

##### 私有函数
- `QString decToBin(int num, int width)`
  - 描述：将十进制数转换为指定宽度的二进制字符串。
  - 参数：
    - `num`：要转换的十进制数。
    - `width`：二进制字符串的宽度。
  - 返回值：转换后的二进制字符串。
- `int binToDec(const QString& binStr)`
  - 描述：将二进制字符串转换为十进制数。
  - 参数：
    - `binStr`：要转换的二进制字符串。
  - 返回值：转换后的十进制数。
- `QString addRoundYihuo(const QString& state, const QString& key)`
  - 描述：执行一轮加密运算，包括状态与密钥之间的异或操作。
  - 参数：
    - `state`：当前状态。
    - `key`：密钥。
  - 返回值：加密后的新状态。

#### 类 `DecryptWidget`

- `DecryptWidget(QWidget *parent = nullptr)`
  - 描述：解密界面的窗口部件构造函数。
  - 参数：
    - `parent`：可选的父级窗口部件，默认为空。

##### 属性
- `QPlainTextEdit* m_cipherTextEdit`
  - 描述：密文的文本编辑框。
- `QLineEdit* m_keyEdit`
  - 描述：密钥的单行文本编辑框。
- `QLabel* m_decryptedLabel`
  - 描述：解密结果的标签。

##### 信号
无

##### 公共槽函数
- `void decrypt()`
  - 描述：执行解密操作的槽函数。

##### 私有函数
无

#### 类 `DecryptWidget`

- `DecryptWidget(QWidget *parent = nullptr)`
  - 描述：解密界面部件的构造函数。
  - 参数：
    - `parent`：可选的父部件，默认为nullptr。

##### 属性
- `QPlainTextEdit* m_cipherTextEdit`
  - 描述：用于输入密文的文本编辑框。
- `QLineEdit* m_keyEdit`
  - 描述：用于输入解密密钥的单行文本编辑框。
- `QLabel* m_decryptedLabel`
  - 描述：用于显示解密结果的标签。

##### 信号
无

##### 公有槽
- `void decrypt()`
  - 描述：执行解密操作的槽函数。

以下是提供的代码的界面文档：

**类名**：double_en_de_cryption

**继承**：QWidget

##### 公有方法

- `double_en_de_cryption(QWidget *parent = nullptr)`
  - 描述：双重加密/解密部件的构造函数。
  - 参数：
    - `parent`：可选的父部件，默认为nullptr。

##### 私有槽函数

- `void double_encrypt()`
  - 描述：执行双重加密操作的槽函数。

- `void double_decrypt()`
  - 描述：执行双重解密操作的槽函数。

#### 类 `encryptCBC`

- `encryptCBC(QWidget *parent = nullptr)`
  - 描述：CBC加密部件的构造函数。
  - 参数：
    - `parent`：可选的父部件，默认为nullptr。

##### 属性

- `QPlainTextEdit* m_plainTextEdit`
  - 描述：用于输入明文的文本编辑框。
- `QLineEdit* m_keyEdit`
  - 描述：用于输入密钥的单行文本编辑框。
- `QLabel* m_cipherLabel`
  - 描述：用于显示加密结果的标签。
- `QLabel* m_ivLabel`
  - 描述：用于显示本轮随机数的标签。

##### 信号

无

##### 公有槽

- `void encrypt()`
  - 描述：执行加密操作的槽函数。

##### 私有方法

- `QString decToBin(int num, int width)`
  - 描述：将十进制数转换为指定位数的二进制字符串。
  - 参数：
    - `num`：要转换的十进制数。
    - `width`：二进制字符串的位数。
  - 返回值：转换后的二进制字符串。

- `int binToDec(const QString& binStr)`
  - 描述：将二进制字符串转换为十进制数。
  - 参数：
    - `binStr`：要转换的二进制字符串。
  - 返回值：转换后的十进制数。

- `QString addRoundYihuo(const QString& state, const QString& key)`
  - 描述：执行一轮密钥异或操作。
  - 参数：
    - `state`：要进行异或操作的状态。
    - `key`：密钥。
  - 返回值：异或操作后的结果。


##### 公有方法

- `encryptCBC(QWidget *parent = nullptr)`
  - 描述：CBC加密部件的构造函数。
  - 参数：
    - `parent`：可选的父部件，默认为nullptr。

##### 私有槽函数

- `void encrypt()`
  - 描述：执行加密操作的槽函数。

#### 类`Tri_2keys_Widget`

##### `Tri_2keys_Widget(QWidget *parent = nullptr)`

创建一个Tri_2keys_Widget对象。

- 参数：
  - `parent` (可选)：父窗口部件指针，默认为nullptr。

##### 槽函数

###### `tri_2keys_encrypt()`

加密按钮的点击事件处理函数。将明文使用AES算法进行三重加密。

##### `tri_2keys_decrypt()`

解密按钮的点击事件处理函数。将密文使用AES算法进行三重解密。

##### 成员变量

##### `m_plainTextEdit`

类型：`QPlainTextEdit*`

明文输入区域部件指针。

##### `m_keyEdit`

类型：`QLineEdit*`

密钥输入框部件指针。

##### `m_cipherTextEdit`

类型：`QPlainTextEdit*`

密文显示区域部件指针。

##### 信号

无。

#### 类`EncryptWidget`

##### 构造函数

##### `EncryptWidget(QWidget *parent = nullptr)`

创建一个EncryptWidget对象。

- 参数：
  - `parent` (可选)：父窗口部件指针，默认为nullptr。

##### 槽函数

###### `encrypt()`

加密按钮的点击事件处理函数。将明文使用AES算法进行加密。

##### 成员变量

###### `m_plainTextEdit`

类型：`QPlainTextEdit*`

明文输入区域部件指针。

###### `m_keyEdit`

类型：`QLineEdit*`

密钥输入框部件指针。

###### `m_cipherLabel`

类型：`QLabel*`

密文显示标签部件指针。

##### 信号

无。