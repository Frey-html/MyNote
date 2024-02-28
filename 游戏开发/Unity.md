   ### 0.学习路线
#### 过一遍Unity基础
* [唐老狮Unity教程](https://www.bilibili.com/video/BV1HX4y1V71E?p=5&vd_source=8636d68797fa4651942df4dc09db7987)
### 1.基础学习
#### 1.基本界面学习
* Scene窗口快捷键 ![[游戏开发/附件/SharedScreenshot.jpg]]
	[Unity中的快捷键](https://blog.csdn.net/LJH_Gemini/article/details/89052132)

* 反射机制 
	[C#中的反射到底用在哪，通俗解释（unity）、反射的概念及用法](https://blog.csdn.net/qq_32175379/article/details/113880100?spm=1001.2101.3001.6650.2&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-2-113880100-blog-82623802.pc_relevant_default&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-2-113880100-blog-82623802.pc_relevant_default&utm_relevant_index=5)
	[Unity3D C#反射底层原理详解](https://zhuanlan.zhihu.com/p/627036528)
	场景本质是配置文件，Unity工作机制就是利用反射(_一个运行的程序查看本身或者其它程序的元数据的行为就叫做反射_)，动态的创建GameObject对象并且关联各种C#脚本对象在其上

#### 2.unity脚本
##### 2.1脚本创建规则
![[屏幕截图 2022-12-05 121440.png]]
* MonoBehaviour![[屏幕截图 2022-12-05 122052.png]]挂载时会通过反射判断是否继承monobehaviour，特性是可以通过反射获得的额外信息![[屏幕截图 2022-12-05 123443.png]]
* 脚本执行先后顺序可以通过Inspector-Execution order设置
* 同一个文件只能有一个类继承MonoBehaviour

##### 2.2生命周期函数
* **帧**：游戏本质是一个死循环，每一次循环更新一次画面，而unity已经实现了循环规则，即生命周期函数
* **生命周期函数**：
	* 所有继承MonoBehaviour的对象都要挂载到GameObject上，生命周期函数就是该GameObject从创建到消亡会通过反射机制自动调用的函数，只有被挂载对象的生命周期函数会被unity调用
	* Unity记录了GameObject挂载了那些脚本，游戏运行时通过反射调用固定名字的函数，反射可以得到私有成员
	* 生命周期函数一般为private或protected，不需要外部调用![[屏幕截图 2022-12-05 134752.png]]
	* 常用函数详解（注意首字母大写）
		1. Awake函数：脚本这个对象自己被创建时调用（可以先不挂载，在游戏运行过程中动态挂载（动态挂载的操作不会被记录下来即只在这一次运行生效），发现挂载时Awake被调用）。类似构造函数，可以在脚本类创建时进行一些初始化
		2. FixedUpdate:
			用于物理更新，重复执行，可以自己设置间隔时间（project settings）
		3. LateUpdate：
			在Update之后执行，**一般用于摄像机位置更新**，因为在两者之间Unity会进行渲染相关的处理，摄像机位置更新一般放在渲染之后
	* 对象在创建后激活，销毁前失活
	**总结：**
	1. 如果不需要使用该生命周期函数就别写
	2. 同一个脚本可以挂载到多个GameObject或在同一个GameObject上挂多次，创建多个对象实例
	3. 如果运行前就设置失活，则该对象生命周期函数不执行

##### 2.3Inspector窗口可编辑变量
* Inspector可显示和编辑的就是脚本成员变量，默认只有public可以显示和编辑，private和protected需要加上强制序列化字段特性[SerializeField],而public可以加上特性[HideInInspector]不显示，**因为继承MonoBehaviour不写构造函数，Inspector中编辑也可用于变量初始化**
* 特性：可以通过反射获得额外的特性信息
* 大部分变量都可通过Inspector编辑，如数组，list，GameObject，但字典和struct，class等自定义类型不行，而加上[System.Serializable]可以使自定义类型可显示编辑(需要加在声明前)
* **注意**：
	* Inspector中修改变量的值就是在修改成员变量，二者通过反射关联
	* 脚本挂载到GameObject后再改变其中脚本文件中变量默认值，该脚本对象实例也即界面上并不会同步改变
	* 运行过程中修改的值不会被保存，如果想要保存运行中的修改，可以在运行中copy component再在运行结束后paste

###### 辅助特性
1. 分组说明：[Header("描述")]可以增加说明，在Inspector中分组显示
2. 悬停注释：[Tooltip("描述")]鼠标指向Inspector中变量时显示描述
3. 间隔：[Space()]将变量之间进行间隔
4. **修饰数值滑条范围**:[Range(start,end)]
5. 多行显示字符串:[Multiline()]设置字符串显示行数
6. 滚动条显示字符串:[TextArea(最少，最多)]最少为最少显示几行，最多为最多为几行时出现滚动条
7. **为变量添加快捷方法**[ContextMenuItem("显示按钮名","方法名")]，方法需要无参无返回值，设置后可在Inspector中右键执行相应方法
8. **为方法添加特性使其能够在Inspector中执行**[ContextMenu("描述")],一般用于调试,点击脚本的三点即可找到并以执行

##### 2.4MonoBehaviour详解
###### 重要成员
1. this.gameObject：继承自Mono，获得所挂载的对象
2. this.transform和this.gameObject.transform效果一致，均为获得挂载对象的transform成员，可通过transform的成员position，eularAngles，lossyScale获得该GameObject位置，角度，大小信息，以及其他成员信息
3. this.enable可以控制脚本对象是否激活
* 也可以通过Inspector窗口关联获得其他脚本挂载的不同的对象，从而获得它们的属性和方法
###### 重要方法
1. 得到自己挂载的object身上的其他单个脚本
	* 通过脚本名字符串获取 this.GetComponent("脚本名")
		返回类型为**Component基类**，可通过as转换为相应派生类，失败返回null
	* 通过类型获取 this.GetComponent(typeof(脚本类型))
		此处传入的实际上是type对象，通过type关键字生成
	* **通过泛型获取** this.GetComponent<脚本类型>() 不用二次转换
	* **只要能够获得场景中object对象或者其挂载的脚本对象，就能够得到其几乎所有信息，包括对象本身和它的所有脚本**
	* 如果有多个同类型脚本，往往无法区分获得的是哪一个
2. 得到自己挂载的object身上的多个脚本
	this.GetComponents< type >()
	一般使用泛型，无参数返回对应类型的对象数组，传入List时会把结果放入List中
3. 得到所挂载对象的子对象（以及子对象的子对象）挂载的脚本, **默认也会找自己是否挂载该脚本** this.GetComponentInChildren()
	* 如果传入True，则失活脚本也会被返回
	* 同理加s找回多个相应类型脚本对象
4. 得到所挂在对象父对象(以及父对象的父对象等)挂载的脚本 this.GetComponentInParent()
5. 尝试获取脚本 this.tryGetComponent<>(out 脚本对象)
	参数为该类型脚本对象，成功时返回true
	更加安全的方法，与常规获取后判断是否为null效果相同
* 知识点：out的使用[out 参数修饰符 - C# 参考 | Microsoft Learn](https://learn.microsoft.com/zh-cn/dotnet/csharp/language-reference/keywords/out-parameter-modifier)

##### 2.5小结
![[屏幕截图 2022-12-06 001250.png]]
![[屏幕截图 2022-12-06 001818.png]]
##### 辅助知识
* 调试：
	如果没有继承MonoBehaviour，使用Debug.Log()，若继承了用print也可以
* 查看源码
	[【Unity】如何查看源码-pudn.com](https://www.pudn.com/news/6316e981f35d77499bb1684f.html#:~:text=%E5%BC%95%E6%93%8E%E6%BA%90%E7%A0%81%E6%9F%A5%E7%9C%8B,%E4%BD%BF%E7%94%A8VS%E6%89%93%E5%BC%80%E8%87%AA%E5%B7%B1%E5%BC%80%E5%8F%91%E7%9A%84%E8%84%9A%E6%9C%AC%EF%BC%8C%E6%8C%89%E4%BD%8FCtrl%E5%8D%95%E5%87%BBMonoBehaviour%E7%B1%BB%EF%BC%88%E6%88%96%E6%8C%89F12%EF%BC%89%EF%BC%8C%E5%B1%95%E5%BC%80%E6%96%87%E4%BB%B6%E5%A4%B4%E9%83%A8%E7%9A%84Region%EF%BC%8C%E6%9F%A5%E7%9C%8B%E6%BA%90%E7%A0%81%E5%AD%98%E6%94%BE%E4%BD%8D%E7%BD%AE%EF%BC%8C%E5%A6%82%E4%B8%8B%E5%9B%BE%E6%89%80%E7%A4%BA%EF%BC%9A)

#### 3.Unity重要组件和API
##### 3.1最小单位GameObject
1. 重要成员变量
	* this.GameObject.name 获取和修改所挂载物体名字
	* this.GameObject.activeSelf 获取所挂载的激活状态，可通过SetActive修改
	* this.GameObject.isStatic 获取所挂载是否是静态对象
	* this.GameObject.layer 获取层级
	* this.GameObject.tag 标签
	* 也可以通过this.GameObject.transform获得transform，this.tansform是通过mono的封装获得的，两者等价
2. **静态方法**
	1. 创建几何体 
		GameObj.CreatePrimitive(PrimitiveType.类型)
		得到创建的object的引用，所创建的object并不属于自身的子object
	2. 查找单个对象（也可以通过inspector直接拖）
		* 通过名字查找 GameObj.Find()
		* 通过标签查找 GameObj.FindWithTag()
		* 通过查找脚本对象查找（效率更低下）
			GameObj.FindObjectOfType<>();
			然后通过该脚本对象.gameObject获取相应对象
		* 特性：
			* 无法找到失活对象
			* 如果场景有多个同名对象，无法找到指定对象
			* **查找需要遍历效率较低，更多可以用Inspector直接关联**
	3. 查找多个对象（只能通过tag去找）
		GameObj.FindGameObjectsWithTag("tag")，返回为数组
	4. **实例化(克隆)对象**
		GameObject.Instantiate()，返回值是创建出来的对象
		**可以是场景中的对象也可以是预设体**
		**最常用的是先在Inspector中关联预设体，再在脚本中创建**
		如果继承了Mono，则不用写GameObject也可以使用这个方法，因为这个方法继承自GameObject和Mono共同的父类unity的Object
	5. 删除对象(也是继承自Object，继承Mono可以直接用)
		* GameObj.Destroy(obj, time)延迟time秒后删除，若只有一个对象参数立即删除
		* 不仅可以删除GameObject对象，还可以删除脚本
		**Destory方法不会立即移除，而是在下一帧时移除并清理内存，只是此时打上标识（为了异步处理降低卡顿）**
		* 如有特殊需求则可以用GameObj.DestroyImmediate()
	6. **过场景不移除**(也是Object提供)
		**默认情况切换场景对象都会被移除**
		GameObject.DontDestroyOnLoad()可以避免脚本所挂载GameObject过场景不被删除
3. 成员方法
	1. 创建空物体
		直接new一个，可以传入字符串作为名字，可传入typeof（脚本类型）添加相应类型的脚本
		注意：如果添加的脚本本身也会创建物体，容易造成循环不断创建新物体
	2. **为对象添加脚本** AddComponent<>()
		返回值为添加上的脚本对象
		继承Mono的脚本类不能直接New，可以通过AddComponent的方式创建对象并挂载
	3. 标签比较 this.gameObject.CompareTag()，或者是直接通过this.gameObject.Tag获取再比较
	4. 设置激活失活 SetActive()
	**注意**：类似于this.gameObject.SendMessage（"func"）（发送消息给自己查找所有脚本，执行其中所有的func方法），BroadcastMessage（）（向自己和自己子对象），SendMessageUpwards(向自己和父对象)通过反射查找效率较低，不如直接得到对象引用并使用

##### 3.2时间相关Time
* 主要用于游戏中位移、计时、时间暂停等，主要用到Time类的一些静态属性
1. 时间比例缩放（一般用于单机游戏）
	Time.timeScale 0为停止，1为正常，2为二倍速以此类推
2. 帧间隔时长
	最近的一帧用了多长时间（秒）
	* Time.deltaTime(受scale影响)
	* Time.unscaleDeltaTime(不受scale影响，实际的帧经过时间)
	**注意到Unity在编辑模式下一帧能跑多快跑多快，而发行后需要限制帧率**
	帧间隔时间主要用来计算位移，根据需求选择参与计算的时间间隔，如果希望暂停时不动的就是用deltaTime，如果正常动则unscale
3. 游戏开始到现在的时间
	Time.time 主要用来单机游戏计时，服务器都以服务器时间为准
	Time.unscaleTime 不受Scale影响时间
4. 物理帧间隔时间 
	Time.fixedDeltaTime
	Time.fixedUnscaledDeltaTime
	* 物理帧间隔在Edit->Project Settings
	->Time中设置
5. 帧数（开始到现在跑了多少帧，即多少次循环）
	Time.frameCount


##### 3.3必不可少的Transform
1. 基础知识点vector3
	用来表示三维坐标系中的一个点，或者是一个向量
	可以用new Vector3（）来创建，默认坐标为0
	* 运算 重载了 + -(向量间) * /(向量和数)
	*  常用静态成员
		Vector3.zero 000 
		Vector3.right 100
		Vector3.foward 001 z轴为面朝向
		Vector3.up 010
		Vector3.Distance() 计算距离
2. 位置信息
	正朝向：z轴正向 右朝向：x轴正向 上朝向：y轴正向
	2. **相对世界坐标系**
		this.transform.position 不会随父子关系改变
		（inspector窗口其实是相对父对象位置）
	2. 相对父对象坐标
		this.transform.localposition
		* 两个坐标一样时要么没有父对象，要么父对象在坐标原点
		* **不能单独改变**position或localposition的x，y，z成员，只能用Vector3对象给其赋值
			如果想要只改变一个时，可以先用Vector3取出position，再修改Vector3，再重新赋值
	3. 通过this.transform.foward/up/right等获取当前面朝向
3. 位移
	1. 自己计算
		this.transform.position += velocity * deltaTime
	2. **API**
		this.transform.Translate()
		参数1表示位移多少，参数2表示相对坐标系默认相对自己的坐标系
		世界坐标系的参数是Space.World,不写或者Space.Self是相对于自己的坐标系
4. 角度和旋转
	1. 角度相关
		transform.angle得到的是四元数，transform.eularAngle得到的是用Vector3表示的欧拉角，单位为度
		相对世界坐标： transform.eularAngle
		相对父对象: transform.localEularAngle
	2. 旋转相关
		一般用API计算
		1. 自转 this.transform.Rotate()
			第一个参数：旋转角度 Vector3
			第二个参数：默认绕自己轴 Space.World则是绕世界的轴
		2. 绕轴转 this.transform.Rotate()
			参数1： 旋转轴 Vector3
			参数2： 旋转的角度
			参数3： 相对的坐标系
		3. 相对于点转 this.Rransform.RotateAround()
			参数1： 点坐标
			参数2： 过点的轴
			参数3： 旋转角度
5. 缩放和看向
	* 缩放：
		相对世界坐标：transform.lossyScale **只读**
		相对父对象: transform.localScale
		也是需要使用Vector3赋值，缩放没有提供额外的API
	* 看向：
		让一个对象的面朝向可以一直看向某一点或对象（即自己的z轴正向指向该点）
		transform.LookAt()
		传入Vector3看向一个点，传入transform看向一个对象
		**父对象LookAt不会带动子对象一起旋转看向**
6. Transform父子关系
	1. 获取和设置父对象（均为transform对象）
		* this.transform.parent 可以修改，设置为null断开父子关系
		* this.transform.SetParent() 
			参数1：父亲transform对象
			**参数2：是否保留世界坐标位置，角度，缩放等信息**
			为true 则世界坐标不变，相对父对象坐标通过运算获得
			为false 则原本世界位置角度信息数值直接拿来作为local，则世界坐标会改变
	2. 舍弃所有子对象(自己和自己儿子，不影响下面的次级关系)
		this.transform.DetachChildren()
	3. 获得子对象(transform)
		1. 按名字查找
			this.transform.Find("")
			**此方法能找到失活对象，而GameObject的静态方法不行**
			**它只能找到自己的儿子，而不会找之下的层级**
			效率高于GameObject.Find()全局查找，但需要得到父对象
		2. 遍历子对象
			子对象个数:this.transform.childCount 失活也算数量
			通过索引遍历: this.transform.GetChild() 0 -> (n-1)
	4. 子对象的操作
		1. 判断一个对象是否是自己父对象
			this.transform.IsChildOf()
		2. 得到自己作为儿子的编号
			this.transform.GetSiblingIndex()
		3. 把自己设置成第一个儿子
			this.transform.SetAsFirstSibling()
		4. 把自己设置成最后一个儿子
			this.transform.SetAsLastSibling()
		5. 把自己设置成指定位置儿子
			this.transform.SetSiblingIndex()
			超出范围时不会报错，自动设置成最大（上溢）或最小（下溢）
7. 坐标变换
	1. 世界坐标转本地坐标    应用为大概判断相对位置
		1. 点的世界坐标系坐标转换为相对本地坐标系的坐标,**受到缩放影响**  this.transform.InverseTransformPoint()
		2. 方向的世界坐标系表示转换为相对本地坐标系表示
			1. this.transform.InverseTransformDirection() **不受缩放影响**
			2. this.transform.InverseTransformVector() **受缩放影响**
	2. **本地坐标转世界坐标**  很方便子对象或特效之类的位置设置（只需考虑和父对象位置关系）
		1. 点相对本地坐标系坐标转世界坐标 **受缩放影响**
			this.transform.TransformPoint()
		2. 方向相对本地坐标系变为世界坐标
			this.transform.TransformDirection()
			this.transform.TransformVector()

#### 4.Input和Screen
##### 鼠标键盘输入 
1. 鼠标在屏幕位置
	屏幕坐标原点是在屏幕左下角，返回值Vector3但z为0
	Input.mousePosition()
2. 检测鼠标输入
	1. 鼠标按下一瞬间进入，0，1，2分别为左右中键
		Input.GetMouseButtonDown(int) 返回为bool 
	2. 鼠标抬起时进入
		Input.GetMouseButtonUp()
	3. 按下抬起都会进入,如果按住则会持续触发
		Input.GetMouseButton()
	4. 中间滚动
		Input.mouseScrollDelta 返回Vector2，其中y表示滚动方向，上滚为1，下滚-1
3. 检测键盘输入
	1. Input.GetKeyDown() 传入一个KeyCode枚举
		有传入字符串的重载，需要小写
	2. Input.GetKeyUp() 检测一次抬起
	3. Input.GetKey() 长按 
4. **检测默认轴输入**
	可以在Project Settings的**Input Manager**内查看相应的Axes名称和属性并且设置，渐变，可以用来控制移动
	* 键盘AD按下时，返回-1到1的变换 Input.GetAxis("Vertical")
	* WS为Horizental  鼠标横向为Mouse X 纵向Mouse Y
	GetAxis是渐变的可提现变换速度，GetAxisRaw使用方法相同，只有-1 0 1整数值
###### 拓展
1. 检测任意键或鼠标
2. 在按下状态 Input.anyKey
3. 是否有任意键按下 input.anyKeyDown    
	可用于改键检测
3. Input.inputString 这一帧的输入 获得改键
4. 获得所连接手柄所有按钮名字
	string[] str = Input.GetJoystickNames()
	检测手柄输入使用Input.GetBottonDown("jump")等
5. 移动设备相关：
	现在手游很少有触摸处理，一般都是虚拟按键
	* Input.touchCount 触摸次数
		Touch t = Input.touches[0]
		可以得到触摸位置和相对上次位置变换(滑动)等
	*  Input.multiTouchEnabled 是否启动多点触控
	*  陀螺仪
		Input.gyro.enabled = true 开启陀螺仪
		Input.gyro.gravity 重力加速度向量，用于计算
		Input.gyro.rotationRate 旋转速度
		Input.gyro.attitude 旋转四元数
##### 屏幕相关Screen
1. 静态属性
	常用的如下：
	2. 屏幕分辨率 
		Resolution r  = Screen.currentResolution;
		r.width r.height
	2. 屏幕窗口当前宽高
		Screen.width Screen.height
	3. 休眠模式
		Screen.sleepTimeout = SleepTimeout.xxx
2. 静态方法
	设置分辨率（pc上）
	Screen.SetResolution(width, height, 是否全屏或全屏模式)

#### 5.Camera
##### 5.1Camera可编辑参数
1. **Clear Flags**
	skybox 天空盒   3d游戏
	Solid Color 颜色填充  2d游戏
	Depth only 只画该层，背景透明   多个摄像机叠加渲染时选择和Depth配合使用
	Dont Clear 不移除上一帧的渲染效果（）
2. **Culling Mask**
	选择性渲染部分层级，可选择渲染哪些层级的对象
3. Projection
	1. Perspective 透视模式 3d游戏
		* Field of view 视口大小
		* FOV 视场轴 决定视口大小是从竖直方向算还是水平方向
		* Physical Camera 有需求时用来模拟真实的摄像机
	2. Othographic 正交模式 2d游戏 没有近大远小
		* size 摄制范围
4. **Clipping Planes** 裁剪平面距离，决定最近和最远能渲染的范围
5. **Depth** 渲染顺序上的深度
	多个摄像机时起效，数字越小越先渲染，后渲染的会覆盖先渲染的内容
	和Depth only结合使用：更高层的摄像机使用depth only时，不渲染背景，即不会用天空盒或纯色完全覆盖之前渲染的层级，达到多个摄像机叠加渲染的效果
	应用之1：底层级渲染游戏，更高层级渲染UI选择Depth only
6. Target Texture 渲染纹理
	右键创建Render Texture，把摄像机画面渲染到RenderTexture上，可用于制作小地图
7. Occulsion Culling 是否启用剔除遮挡
	启用则不渲染被完全遮挡的物体，节约计算资源
8. ViewRect 视口范围，屏幕上绘制摄像机的位置
	0-1为宽高百分比，主要用于双摄相机游戏

##### 5.2CameraAPI相关
1. 重要静态成员
	1. 获取主摄像机 Camera.main() 需要有Tag为Main Camera的摄像机
		如果有两个，不会报错但不能确定获得的是哪个
	2. 获取摄像机数量 Camera.allCamerasCount
	3. 得到所有摄像机
		Camera[] cameras = Camera.allCameras
2. 渲染相关委托
	1. Camera.onPreCull +=  (c) =>{} 剔除前处理的委托
	2. Camera
	

### Tips
#### Object
* unity中的Object是Unity自己在命名空间UnityEngine下编写的但也是派生于C#类万物之父Object的一个自定义类，C#的Object在System里，二者是同名类

### 疑问


### 遇到的问题
#### unityHub无法正常打开
[Unity2020或Unity2019安装后无法启动](https://blog.csdn.net/lzw0321/article/details/118540014#:~:text=%E5%8E%9F%E5%9B%A0%20%E9%80%9A%E8%BF%87%E6%9F%A5%E7%9C%8Bunity%20hub%E7%9A%84%E6%97%A5%E5%BF%97%E5%8F%91%E7%8E%B0Unity%20%E5%90%AF%E5%8A%A8%E7%9A%84%E6%97%B6%E5%80%99%E4%BC%9A%E6%A3%80%E6%9F%A5,liences%EF%BC%8C%E5%A6%82%E6%9E%9C%E4%B8%8D%E5%90%88%E8%A7%84%E5%88%99%E6%97%A0%E6%B3%95%E5%90%AF%E5%8A%A8%E3%80%82%20unity%20hub%E7%9A%84%E6%97%A5%E5%BF%97%E5%AD%98%E6%94%BE%E5%9C%A8%EF%BC%9AC%3AUsersqingAppDataRoaming%5B%26Un%26%5DityHublogs%EF%BC%8C%E6%8C%89%E4%B8%8BWin%2BR%EF%BC%8C%E5%9C%A8%E8%BF%90%E8%A1%8C%E4%B8%AD%E8%BE%93%E5%85%A5%EF%BC%9A%25USERPROFILE%25AppDataRoaming%5B%26Un%26%5DityHublogs%EF%BC%8C%E5%9B%9E%E8%BD%A6%E5%B0%B1%E5%8F%AF%E4%BB%A5%E6%89%93%E5%BC%80unity%20hub%20log%E7%9B%AE%E5%BD%95%E3%80%82)
[如何把Unity卸载干净？](https://blog.csdn.net/weixin_46218781/article/details/104959896)
自己测试过程中，unityhub安装到D盘会闪退，C盘则正常启动
