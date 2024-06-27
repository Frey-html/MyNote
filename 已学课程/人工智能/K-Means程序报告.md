学号：3190102196  姓名：展翼飞

#### 问题重述
本问题为异常值检测，异常值检测（outlier detection ）是一种数据挖掘过程，用于发现数据集中的异常值并确定异常值的详细信息。我们需要设计算法读取并找出给定数据中的异常值。

#### 设计思想
关键为了解 KMeans、PCA算法，首先构造一些额外的特征用于探究异常点检测，各个特征之间并不是相互独立的，存在一定程度上的相关性，后采用 PCA对数据特征的变换，寻找特征空间中，数据分布方差最大的方向，称为特征方向或主成分方向，选择其中特征值较大的几个特征方向，将数据点投影到这些方向上，完成数据降维。降维后按照样本之间的距离大小，将样本集划分为 K 个簇。  让簇内的点尽量紧密的连在一起，而让簇间的距离尽量的大即使用KMeans 聚类方法完成检测。

#### 代码内容
Kmeans算法：使用kmeans++选择kmeans初始簇中心
```python

    def kmeans_plus_plus_initialize(self, x):
        # K-Means++ 初始化方法
        centers = [x[np.random.randint(len(x))]]  # 从 x 中随机选择一个点作为第一个簇中心
        for _ in range(1, self.n_clusters):
            # 计算每个点到当前已选中簇中心的最短距离的平方
            distances = np.array([min([np.linalg.norm(x[i] - c)**2 for c in centers]) for i in range(len(x))])
            # 根据距离的概率分布，随机选择下一个簇中心
            probabilities = distances / np.sum(distances)
            next_center_index = np.random.choice(len(x), p=probabilities)
            centers.append(x[next_center_index])
        return np.array(centers)

    def fit(self, x):
        """
        用fit方法对数据进行聚类
        :param x: 输入数据
        :best_centers: 簇中心点坐标 数据类型: ndarray
        :best_labels: 聚类标签 数据类型: ndarray
        :return: self
        """
    
        if isinstance(x, pd.DataFrame):  # 检查输入是否为DataFrame
            x = x.values  # 将DataFrame转换为NumPy数组
            
        best_inertia = None  # 初始化最佳簇的总距离平方和为None，用于之后的比较
        best_centers = None  # 初始化最佳簇中心为None
        best_labels = None   # 初始化最佳标签为None

        for _ in range(self.n_init):  # 重复 n_init 次以找到最佳簇划分
            # 使用 K-Means++ 初始化方法选择初始簇中心
            centers = self.kmeans_plus_plus_initialize(x)

            for _ in range(self.max_iter):  # 迭代 max_iter 次以优化簇中心
                # 计算每个数据点到各个簇中心的距离
                distances = np.linalg.norm(x[:, np.newaxis] - centers, axis=2)
                # 根据最小距离分配簇标签，labels 表示每个数据点所属的簇的索引
                labels = np.argmin(distances, axis=1)

                # 计算新的簇中心，即每个簇中所有数据点的均值
                new_centers = np.array([x[labels == i].mean(axis=0) for i in range(self.n_clusters)])

                # 检查簇中心是否收敛，即是否和上一次的簇中心相同
                if np.all(centers == new_centers):
                    break
                centers = new_centers  # 更新簇中心

            # 计算当前簇的总距离平方和，作为簇划分的评估指标
            inertia = np.sum((x - centers[labels]) ** 2)

            # 选择总距离平方和最小的一次结果作为最佳结果
            if best_inertia is None or inertia < best_inertia:
                best_inertia = inertia
                best_centers = centers
                best_labels = labels

        self.cluster_centers_ = best_centers  # 存储最佳簇中心
        self.labels_ = best_labels  # 存储最佳标签
        return self

```
**Kmeans++**:
1. **选择第一个中心点**：随机选择一个数据点作为第一个聚类中心。
2. **选择其他中心点**：对于剩余的未被选中的数据点，计算每个数据点与已选中的聚类中心的最短距离（即距离最近的聚类中心的距离），然后以一定的概率选择下一个聚类中心。距离当前聚类中心越远的点，被选中的概率就越大。这样做的目的是保证新选取的聚类中心与已选中的聚类中心之间的距离尽可能地远。
3. **重复选择过程**：重复上述步骤，直到选取足够数量的聚类中心。
**数据处理策略**：
	使用PCA算法对
	|cpc|cpm|cpc X cpm|cpc * cpm|hours|daylight|
	 六列数据进行降维降到2维，此时数据较易看出分簇情况，且测试中得分也高于3维
	 ![[Pasted image 20240518185942.png]]
![[Pasted image 20240518190004.png]]
	最终选择聚类数目为7

#### 实验结果
![[Pasted image 20240518185416.png]]