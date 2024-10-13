[简介 - Git教程 - 廖雪峰的官方网站 (liaoxuefeng.com)](https://liaoxuefeng.com/books/git/introduction/index.html)
[note.isshikih.top/tech_accu/tool/Git/Workflow/#git-workflow_1](https://note.isshikih.top/tech_accu/tool/Git/Workflow/#git-workflow_1)

### 一、Git 常用命令及其用法

#### 1. 基本的 Git 操作命令

- **`git init`**: 初始化一个新的 Git 仓库（将当前目录变成 Git 仓库）。
  
  ```bash
  git init
  ```

- **`git clone <repo>`**: 从远程仓库克隆（复制）一个仓库到本地。
  
  ```bash
  git clone https://github.com/user/repo.git
  ```

- **`git status`**: 查看当前工作目录的状态，查看哪些文件被修改、暂存，哪些文件未被跟踪等。

  ```bash
  git status
  ```

- **`git add <file>`**: 将指定文件添加到暂存区（准备提交）。你可以指定文件名或使用 `.` 表示所有文件。

  ```bash
  git add <file>      # 添加单个文件
  git add .           # 添加所有更改的文件
  ```

- **`git commit -m "<message>"`**: 提交暂存区的更改到本地仓库，并附带提交消息。

  ```bash
  git commit -m "Initial commit"
  ```

- **`git log`**: 查看提交历史记录。

  ```bash
  git log
  ```

- **`git diff`**: 显示工作区的未提交更改。

  ```bash
  git diff
  ```

- **`git checkout <branch>`**: 切换到指定的分支或检出特定的提交。

  ```bash
  git checkout main   # 切换到分支
  git checkout <commit-hash>  # 切换到特定提交
  ```

- **`git branch <branch-name>`**: 创建新分支。

  ```bash
  git branch new-feature
  ```

- **`git branch -d <branch-name>`**: 删除分支。

  ```bash
  git branch -d old-branch
  ```

- **`git merge <branch>`**: 合并指定分支到当前分支。

  ```bash
  git merge new-feature
  ```

#### 2. 远程仓库相关命令

- **`git remote add <name> <url>`**: 添加远程仓库，通常 `name` 是 `origin`，`url` 是远程仓库地址。

  ```bash
  git remote add origin https://github.com/user/repo.git
  ```

- **`git remote -v`**: 查看远程仓库的详细信息（fetch 和 push 的地址）。

  ```bash
  git remote -v
  ```

- **`git push`**: 将本地提交推送到远程仓库。

  ```bash
  git push origin main   # 推送 main 分支到远程 origin 仓库
  ```

- **`git pull`**: 从远程仓库拉取最新的更改并合并到当前分支。

  ```bash
  git pull origin main   # 拉取并合并远程 main 分支的最新更改
  ```

- **`git fetch`**: 从远程仓库下载更改，但不合并到本地分支（需手动合并）。

  ```bash
  git fetch origin       # 仅下载最新的远程更改
  ```

- **`git push -u origin <branch>`**: 推送当前分支并设置上游分支。

  ```bash
  git push -u origin main   # 推送并设置上游分支
  ```

- **`git remote remove <name>`**: 删除远程仓库。

  ```bash
  git remote remove origin
  ```

#### 3. Git 分支管理命令

- **`git branch`**: 查看当前分支列表及当前所在的分支。

  ```bash
  git branch
  ```

- **`git checkout -b <branch-name>`**: 创建并切换到新分支。

  ```bash
  git checkout -b new-branch
  ```

- **`git branch -m <new-name>`**: 重命名当前分支。

  ```bash
  git branch -m new-name
  ```

- **`git merge <branch-name>`**: 将另一个分支合并到当前分支。

  ```bash
  git merge feature-branch
  ```

- **`git rebase <branch-name>`**: 将当前分支的提交放在目标分支的最新提交之后，重新应用提交。

  ```bash
  git rebase main
  ```

#### 4. Git 恢复与撤销命令

- **`git reset --hard <commit-hash>`**: 将当前分支重置到指定的提交，丢弃所有未提交的更改。

  ```bash
  git reset --hard abc1234
  ```

- **`git revert <commit-hash>`**: 创建一个新的提交来撤销某次提交的更改。

  ```bash
  git revert abc1234
  ```

- **`git stash`**: 暂存当前的工作区修改，保存它们以便以后恢复。

  ```bash
  git stash
  ```

- **`git stash pop`**: 恢复最近的 `stash` 内容。

  ```bash
  git stash pop
  ```

### 二、远程仓库与本地仓库的连接方式

Git 是一个分布式系统，因此可以有多种初始化和连接远程仓库的方式。接下来我们来详细讨论如何在不同场景下连接远程仓库。

#### 场景 1: 本地仓库已经初始化，远程仓库还没创建

1. **本地初始化 Git 仓库**：
   
   首先，初始化你的本地 Git 仓库，并进行第一次提交：

   ```bash
   git init
   git add .
   git commit -m "Initial commit"
   ```

2. **在 GitHub（或其他平台）上创建远程仓库**：
   
   登录 GitHub，创建一个新的远程仓库，但保持仓库为空（不要添加任何文件，如 README）。

3. **将远程仓库与本地仓库关联**：

   使用 `git remote add` 命令将本地仓库关联到远程仓库：

   ```bash
   git remote add origin https://github.com/user/repo.git
   ```

4. **推送本地仓库到远程仓库**：

   将本地的代码推送到远程仓库：

   ```bash
   git push -u origin main
   ```

#### 场景 2: 本地仓库未初始化，远程仓库已存在

1. **克隆远程仓库**：
   
   如果你在 GitHub 上已经有了远程仓库，可以直接克隆该仓库到本地：

   ```bash
   git clone https://github.com/user/repo.git
   ```

   这会将远程仓库的代码下载到本地，并且自动配置好远程仓库 `origin`。

2. **在本地进行开发**：

   克隆后，你可以在本地进行开发，创建新文件、修改文件等。

3. **提交本地更改**：

   当你完成开发时，将更改提交到本地仓库：

   ```bash
   git add .
   git commit -m "New feature implemented"
   ```

4. **推送更改到远程仓库**：

   将本地的更改推送到远程仓库：

   ```bash
   git push origin main
   ```

#### 场景 3: 本地和远程仓库都未初始化

1. **初始化本地仓库**：

   在本地文件夹中初始化 Git 仓库并进行首次提交：

   ```bash
   git init
   git add .
   git commit -m "Initial commit"
   ```

2. **创建远程仓库**：

   登录 GitHub 或其他平台，创建一个新的远程仓库。

3. **将本地仓库与远程仓库关联**：

   将本地仓库与远程仓库进行关联：

   ```bash
   git remote add origin https://github.com/user/repo.git
   ```

4. **推送到远程仓库**：

   将本地代码推送到远程仓库：

   ```bash
   Git push -u origin main
   ```

#### 场景 4: 本地和远程仓库已存在，但未关联

1. **查看当前远程仓库**：

   首先检查你的本地仓库是否已经配置了远程仓库：

   ```bash
   Git remote -v
   ```

2. **添加远程仓库**：

   如果本地仓库没有关联远程仓库，可以通过以下命令添加：

   ```bash
   git remote add origin https://github.com

/user/repo. Git
   ```

3. **推送本地代码到远程仓库**：

   将本地代码推送到远程仓库：

   ```bash
   Git push -u origin main
   ```

### 三、总结

- Git 常用命令包括初始化仓库、提交、查看状态、推送和拉取等操作。分支管理、合并、恢复与撤销等命令也在开发中非常常用。
- 远程仓库与本地仓库的连接方式取决于是否已经初始化本地或远程仓库。可以根据具体情况使用 `git remote add`、`git push`、`git clone` 等命令来建立连接。
