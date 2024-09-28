[[技术设计.pdf]]

### 重点设计
#### UI 基类，派生类，管理器与触发器添加
##### 基类 UIBase 与触发器监听类 UIEventTrigger
提供基础方法与 Register 事件注册函数，子类可通过 Register 方法为按钮等组件挂上一个事件监听组件 UIEventTrigger（如果子类没有挂上该组件）并返回该组件，该组建实现了 IPointerClickHandler 接口
```C#
public class UIBase : MonoBehaviour

{

    // 注册事件，返回UIEventTrigger对象

    public UIEventTrigger Register(string name)

    {
        // 查找当前UI对象的子对象
        Transform tf = transform.Find(name);
        // 获取或添加UIEventTrigger组件到找到的子对象
        return UIEventTrigger.Get(tf.gameObject);
    }

    // 显示UI，虚方法可以被子类重写
    public virtual void Show()
    {
        // 设置当前游戏对象为激活状态
        gameObject.SetActive(true);
    }

  
    // 隐藏UI，虚方法可以被子类重写
    public virtual void Hide()
    {
        // 设置当前游戏对象为非激活状态
        gameObject.SetActive(false);
    }

  

    // 关闭UI，虚方法可以被子类重写
    public virtual void Close()
    {
        // 调用UIManager的实例，关闭当前游戏对象的UI
        UIManager.Instance.CloseUI(gameObject.name);
    }
}
```
```C#
// 事件监听器类，实现了IPointerClickHandler接口，用于监听鼠标点击事件

public class UIEventTrigger : MonoBehaviour, IPointerClickHandler

{
    // 事件委托，当对象被点击时触发
    public Action<GameObject, PointerEventData> onClick;

    // 静态方法，用于获取或添加UIEventTrigger组件到指定的GameObject
    public static UIEventTrigger Get(GameObject obj)
    {
        // 尝试获取GameObject上的UIEventTrigger组件
        UIEventTrigger trigger = obj.GetComponent<UIEventTrigger>();
        // 如果组件不存在，添加一个新的UIEventTrigger组件
        if (trigger == null)
        {
            trigger = obj.AddComponent<UIEventTrigger>();
        }
        // 返回UIEventTrigger组件
        return trigger;
    }
  

    // 实现IPointerClickHandler接口的方法，当对象被点击时调用
    public void OnPointerClick(PointerEventData eventData)
    {
        // 如果onClick事件不为空，触发事件并传递当前对象和事件数据
        if (onClick != null)
        {
            onClick(gameObject, eventData);
        }
    }
}
```

##### UI 管理器
控制 UI 的加载，显示，关闭等，核心为 Find 方法，通过多态存储各种UI 在 List 中并根据时机调用，动态加载
```C#
public UIBase ShowUI<T>(string uiName) where T : UIBase

    {
        UIBase ui = Find(uiName);
        if (ui == null)
        {
            //集合中没有 需要从Resources/UI文件夹中更新
            GameObject obj = Instantiate(Resources.Load("UI/" + uiName) , canvasTf) as GameObject;

            //改名字
            obj.name = uiName;
            //添加需要的脚本
            ui = obj.AddComponent<T>();
            //添加到集合进行储存
            uiList.Add(ui);
        }
        else
        {
            //显示
            ui.Show();
        }
        return ui;
    }
```
##### 核心战斗 UI 类
通过与回合管理器 FightManager，抽卡管理器 FightCardManager 等交互，实现战斗界面血量、卡牌实体创建、弃牌动画等 UI 效果以及回合结束按钮
```C#
//创建卡牌实体

    public void CreateCardItem(int count)
    {
        for(int i = 0; i < count;i++){
            //如果抽卡过程中抽卡堆为0，则洗牌，将弃牌堆卡牌置入抽排堆
            if(FightCardManager.Instance.cardList.Count == 0){
                FightCardManager.Instance.Shuffle();
                //更新牌堆数量UI
                UIManager.Instance.GetUI<FightUI>("FightUI").UpdateUsedCardCount();
                UIManager.Instance.GetUI<FightUI>("FightUI").UpdateCardCount();
            }
            GameObject obj = Instantiate(Resources.Load("UI/CardItem"), transform) as GameObject;
            obj.GetComponent<RectTransform>().anchoredPosition = new UnityEngine.Vector2(-1000, -700);
            //var item = obj.AddComponent<CardItem>();
            string cardId = FightCardManager.Instance.DrawCard();
            Dictionary<string, string> data = GameConfigManager.Instance.GetCardById(cardId);
            CardItem item = obj.AddComponent(System.Type.GetType(data["Script"])) as CardItem;
            item.Init(data);
            cardItemList.Add(item);
        }
        //更新卡堆UI
        UIManager.Instance.GetUI<FightUI>("FightUI").UpdateUsedCardCount();
        UIManager.Instance.GetUI<FightUI>("FightUI").UpdateCardCount();
        //更新卡牌位置
        UIManager.Instance.GetUI<FightUI>("FightUI").UpdateCardItemPos();
    }


    // 更新卡牌的位置
    public void UpdateCardItemPos()
    {
        // 计算每张卡牌之间的水平偏移量，使它们在水平方向上均匀分布
        float offset = 800.0f / cardItemList.Count;
        // 计算起始位置，使卡牌项在水平方向上居中显示，并且位于屏幕底部
        UnityEngine.Vector2 startPos = new UnityEngine.Vector2(
            -cardItemList.Count / 2.0f * offset + offset * 0.5f, // X 坐标
            -700); // Y 坐标
        // 遍历卡牌项列表，逐个更新它们的位置
        for(int i = 0; i < cardItemList.Count; i++)
        {
            // 使用 DOTween 库中的 DOAnchorPos 方法来动画地更新卡牌项的锚点位置
            cardItemList[i].GetComponent<RectTransform>().DOAnchorPos(startPos, 1.0f);
            // 更新起始位置的 X 坐标，以便下一个卡牌项可以正确地排列在其右侧
            startPos.x = startPos.x + offset;
        }
    }

  

    //将卡牌物体置入弃牌堆
    public void RemoveCard(CardItem item)
    {
        AudioManager.Instance.PlayEffect("Cards/cardShove"); //出牌音效
        item.enabled = false; //禁用卡牌逻辑
        //添加到弃牌堆集合
        FightCardManager.Instance.usedCardList.Add(item.data["Id"]);
        //更新使用弃牌堆卡牌数量
        noCardCountTxt.text = FightCardManager.Instance.usedCardList.Count.ToString();
        //从手牌堆中移除该卡牌
        cardItemList.Remove(item);
        //刷新手牌渲染
        UpdateCardItemPos();
        //播放卡牌移动到弃牌堆动画
        item.GetComponent<RectTransform>().DOAnchorPos(new Vector2(1000, -700), 0.25f);
        item.transform.DOScale(0, 0.25f);
        Destroy(item.gameObject, 1);
    }
```



#### 游戏配置文件读取
##### 配置文件类
将游戏的一种配置数据读入成字典的列表，列表中每一个字典存储一个条目的键值对
```C#
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

  

//游戏配置表类 每个对象对应一个txt配置表

public class GameConfigData

{
    private List<Dictionary<string, string>> dataDic;//存储配置表中的所有数据
    public GameConfigData(string str)
    {
        dataDic = new List<Dictionary<string, string>>();
        //换行切割
        string[] lines = str.Split('\n');
        //第一行是存储数据的类型
        string[] title = lines[0].Trim().Split('\t');//tab切割
        //从第三行下标2开始 开始遍历数据 第二行数据时解释说明
        for(int i=2; i<lines.Length; i++)
        {
            Dictionary<string,string> dic = new Dictionary<string,string>();
            string[] tempArr = lines[i].Trim().Split('\t');
  
            for(int j=0; j<tempArr.Length; j++)
            {
                dic.Add(title[j], tempArr[j]);
            }
            dataDic.Add(dic);
        }
    }

  

    public List<Dictionary<string, string>> GetLines()
    {
        return dataDic;
    }

  
    public Dictionary<string, string> GetOneById(string id)
    {
        for (int i = 0; i < dataDic.Count; i++)
        {
            Dictionary<string, string> dic = dataDic[i];
            if (dic["Id"] == id)
            {
                return dic;
            }
        }
        return null;
    }

  
    public int GetMaxId()
    {
        int i = dataDic.Count - 1;
        return int.Parse(dataDic[i]["Id"]);
    }
}
```

##### 配置文件管理器
读取所有类型的配置文件并存储，提供接口给游戏逻辑读取条目，在一开始初始化时读入所有数据
```C#
//用户信息管理器（拥有的卡牌等信息、金币）
public class GameConfigManager

{
    public static GameConfigManager Instance = new GameConfigManager();
    private GameConfigData cardData; // 卡牌表
    private GameConfigData enemyData;// 敌人表
    private GameConfigData levelData;// 关卡表
    private GameConfigData cardTypeData;// 卡牌类型表
    private TextAsset textAsset;

    public void Init()
    {
        textAsset = Resources.Load<TextAsset>("Data/card");
        cardData = new GameConfigData(textAsset.text);
        textAsset = Resources.Load<TextAsset>("Data/enemy");
        enemyData = new GameConfigData(textAsset.text);
        textAsset = Resources.Load<TextAsset>("Data/level");
        levelData = new GameConfigData(textAsset.text);
        textAsset = Resources.Load<TextAsset>("Data/CardType");
        cardTypeData = new GameConfigData(textAsset.text);
    }

    public List<Dictionary<string,string>> GetCardLines()
    {
        return cardData.GetLines();
    }

    public List<Dictionary<string, string>> GetEnemyLines()
    {
        return enemyData.GetLines();
    }

    public List<Dictionary<string, string>> GetLevelLines()
    {
        return levelData.GetLines();
    }

    public Dictionary<string,string> GetCardById(string id)
    {
        return cardData.GetOneById(id);
    }

  

    public Dictionary<string, string> GetEnemyById(string id)
    {
        return enemyData.GetOneById(id);
    }

    public Dictionary<string, string> GetLevelById(string id)
    {
        return levelData.GetOneById(id);
    }

    public Dictionary<string, string> GetCardTypeById(string id){
        return cardTypeData.GetOneById(id);
    }

  
    public int GetMaxLevelId()
    {
        return levelData.GetMaxId();
    }
}
```



#### 游戏回合机制实现
##### 敌方回合：怪物管理，实例化与协程动作播放
主要为 EnemyManager 类与 Enemy 类



##### 我方回合：卡牌管理，实例化与射线检测
卡牌管理：rolemanager 定义初始牌堆，fightcardManager 管理手牌堆弃牌堆与抽牌弃牌洗牌，以及 cardItem 基类（实现大部分卡牌通用效果，拖拽，悬停放大，音效等交互效果）与派生类（实现对应卡牌独特效果）
##### CardItem 基类--卡牌交互基本效果（对象初始化，悬停，拖拽，使用判定）
```C#
//拖拽中
    public virtual void OnDrag(PointerEventData eventData)
    {
        // 存储转换后的局部坐标
        Vector2 pos;
        // 将屏幕坐标转换为父容器的局部坐标
        if (RectTransformUtility.ScreenPointToLocalPointInRectangle(
            transform.parent.GetComponent<RectTransform>(), // 父容器的 RectTransform
            eventData.position, // 当前指针的位置（屏幕坐标）
            eventData.pressEventCamera, // 按下事件时的相机
            out pos // 输出参数，将转换后的局部坐标存储在 pos 中
            ))
        {
            // 如果转换成功，将当前对象的锚点位置设置为转换后的局部坐标
            transform.GetComponent<RectTransform>().anchoredPosition = pos;
        }
    }
```

##### FightCard 类
当然可以！这段代码实现了一个攻击卡的逻辑，当用户点击攻击卡时，会显示一条曲线界面，并允许用户拖动鼠标来指定攻击方向。在拖动过程中，代码会持续更新曲线的终点位置，并进行射线检测以确定是否命中敌人。下面是详细的分析：

######  代码结构分析

1. **继承关系**：
   - `AttackCardItem` 类继承自 `CardItem` 类，并实现了 `IPointerDownHandler` 接口。这意味着它能够处理鼠标点击事件。

2. **协程逻辑**：
   - 当用户点击攻击卡时，会触发 `OnPointerDown` 方法，该方法中启动了一个名为 `OnMouseDownRight` 的协程。
   - 协程 `OnMouseDownRight` 的主要逻辑是在每次 `Update` 时检查鼠标的当前位置，并更新曲线界面的终点位置。
   - 如果用户释放鼠标右键，则协程会结束，并恢复鼠标显示和关闭曲线界面。

3. **射线检测**：
   - `CheckRayToEnemy` 方法用于执行射线检测，判断是否命中敌人。
   - 如果命中敌人，则记录该敌人，并在用户点击鼠标左键时触发攻击逻辑。
   - 如果没有命中敌人，则取消选中之前的敌人（如果有）。

###### 为什么使用协程

在这段代码中，使用协程的主要原因有以下几点：

1. **持续更新曲线位置**：
   - 用户拖动鼠标时，需要不断更新曲线界面的终点位置。协程 `OnMouseDownRight` 每次 `yield return null` 后都会在下一帧继续执行，从而实现连续更新。
   
2. **避免阻塞主线程**：
   - 如果不在协程中实现这个逻辑，而是使用循环或其他方式，可能会导致大量的计算工作在单次调用中完成，从而阻塞主线程，影响游戏性能和响应速度。
   
3. **优雅地处理鼠标释放事件**：
   - 当用户释放鼠标右键时，协程可以立即退出，并执行清理工作（如恢复鼠标显示、关闭曲线界面等）。

###### 代码逻辑详解

1. **`OnPointerDown` 方法**：
   - 播放声音。
   - 显示曲线界面。
   - 设置曲线界面的起始位置。
   - 隐藏鼠标。
   - 停止所有协程（确保之前的逻辑不会干扰当前操作）。
   - 启动 `OnMouseDownRight` 协程来处理鼠标拖动逻辑。

2. **`OnMouseDownRight` 协程**：
   - 在一个无限循环中，检查鼠标右键是否被按下。如果是，则跳出循环。
   - 使用 `RectTransformUtility.ScreenPointToLocalPointInRectangle` 将屏幕坐标转换为局部坐标，并更新曲线界面的终点位置。
   - 调用 `CheckRayToEnemy` 方法进行射线检测。
   - 每次循环结束时，使用 `yield return null` 将控制权交回给主线程，等待下一帧继续执行。

3. **`CheckRayToEnemy` 方法**：
   - 创建射线并执行射线检测。
   - 如果命中敌人，则记录该敌人并选中它。
   - 如果用户点击鼠标左键，则触发攻击逻辑。
   - 如果没有命中敌人，则取消选中之前的敌人（如果有）。

###### 为什么使用 `yield return null`

在协程中使用 `yield return null` 的主要作用是：

- **暂停当前协程**：将当前协程的执行暂停，直到下一帧再继续执行。
- **避免阻塞主线程**：确保协程不会在单帧内占用过多时间，从而影响游戏性能。

通过这种方式，协程可以在每次 `Update` 时继续执行，从而实现连续的逻辑处理，如持续更新曲线位置。

###### 回合切换机制，获胜失败判定
主要为回合基类 FightUnit 与其派生回合类，以及管理器 FightManager




#### 奖励机制




#### 自制工具
Excel 转 txt