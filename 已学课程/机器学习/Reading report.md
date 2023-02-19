展翼飞 3190102196

* **Describe what problem the article is trying to solve**
	The article is trying to propose a new algorithm for support vector machines which can be parallelized efficiently and scales to very large problems with 
	hundreds of thousands of training vectors.


* **Describe the main idea of the article to solve the problem**
	The main idea of the article is that instead of analyzing the whole training set in one optimization step, the data are split into subsets and optimized separately with multiple SVMs. The partial results are combined and filtered again in a ‘Cascade’ of SVMs, until the global optimum is reached. The Cascade SVM can be spread over multiple processors with minimal communication overhead and requires far less memory, since the kernel matrices are much smaller than for a regular SVM.In order to break through the limits of today’s SVM implementations the authors developed a distributed architecture, where smaller optimizations are solved independently and can be spread over multiple processors, yet the ensemble is guaranteed to converge to the globally optimal solution。

* **Briefly describe the difference between SVM and Cascade SVM**
	* Support Vector Machines : they are powerful classification and regression tools, but their compute and storage requirements increase rapidly with the number of training vectors, putting many problems of practical interest out of their reach.tors from the rest of the training data. General-purpose QP solvers tend to scale with the cube of the number of training vectors (O(k3 )).
	* Cascade SVM : Eliminating non-support vectors early from the optimization proved to be an effective strategy for accelerating SVMs. Using this concept we developed a filtering process that can be parallelized efficiently. After evaluating multiple techniques, such as projections onto subspaces (in feature space) or clustering techniques, we opted to use SVMs as filters. This makes it straightforward to drive partial solutions towards the global optimum, while alternative techniques may optimize criteria that are not directly relevant for finding the global solution.


* **Describe what experiments are designed in the article, which datasets are compared, what are the baselines, and what advantages Cascade SVM has**
	* **Experiments:**
		* Counting the number of kernel evaluations required for one pass.A 9-layer Cascade requires only about 30% as many kernel evaluations as a single SVM for 100,000 training vectors.
		* Experiment FOREST problem with 100 vectors to findout the relationship of Number of Kernel evaluations (requests and actual, with a cache size of 800MB) for different numbers of layers in the Cascade (single pass).
		* Training times for a large data set with 1,016,736 vectors (MNIST wasexpanded by warping the handwritten digits). A Cascade with 5 layers is executed on a Linux cluster with 16 machines (AMD 1800, dual processors, 2GB RAM permachine). The solution converges in 3 iterations. Shown are also the maximum number of training vectors on one machine and the number of support vectors in the last stage. W: optimization function; Acc: accuracy on test set. Kernel: RBF, gamma=1; C=50.
		* Speed-up for a parallel implementation of the Cascades with 1 to 5 layers (1 to 16 SVMs, each running on a separate processor), relative to a single SVM: single pass (left), fully converged (middle) (MNIST, NORB: 3 iterations, FOREST: 5 iterations). On the right is the generalization performance of a 5-layer Cascade, measured after each iteration. For MNIST and NORB, the accuracy after one pass is the same as after full convergence (3 iterations). For FOREST, the accuracy improves from 90.6% after a single pass to 91.6% after convergence (5iterations). Training set sizes: MNIST: 60k, NORB: 48k, FOREST: 186k.
	* **DataSet**
		MNIST: handwritten digits, d=784 (28x28 pixels); training: 60,000; testing: 10,000;classes: odd digits - even digits; http://yann.lecun.com/exdb/mnist.
		FOREST: d=54; class 2 versus rest; training: 560,000; testing: 58,100. ftp://ftp.ics.uci.edu/pub/machine-learning-databases/covtype/covtype.info.
		NORB: images, d=9,216 ; trainingr=48,600; testing=48,600; monocular; merged class 0 and 1 versus the rest. http://www.cs.nyu.edu/~ylclab/data/norb-v1.0
	* **Baselines**
		* single SVM or the number of layers
	* **Advantages** : it can reduce compute- as well as storage-requirements
		* It requires far less memory than a single SVM, because the size of the kernel matrix scales with the square of the active set.
		* A simulation on a single processor can produce a speed-up of 5x to 10x or more, depending on the available memory size. F


* **Please write down one of your questions about the proposed method**
	How strong is the ability of Cascade SVM to generalize, and what specific areas can it be used for