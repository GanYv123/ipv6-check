# ElaScrollPage Class API Documentation

`ElaScrollPage` 是一个带有面包屑导航的可滚动页面管理组件，支持多中心页面切换、自定义控件插入及滚动手势。

---

## 目录

- [构造与析构](#构造与析构)
- [公共方法](#公共方法)
  - [addCentralWidget](#addcentralwidget)
  - [setCustomWidget](#setcustomwidget)
  - [getCustomWidget](#getcustomwidget)
  - [navigation](#navigation)
  - [setPageTitleSpacing](#setpagetitlespacing)
  - [getPageTitleSpacing](#getpagetitlespacing)
  - [setTitleVisible](#settitlevisible)
- [信号](#信号)
- [内部机制及路由](#内部机制及路由)
- [依赖组件](#依赖组件)

---

## 构造与析构

### `ElaScrollPage(QWidget* parent = nullptr)`

- **功能**：构造函数，初始化页面和内部布局。
- **参数**：
  - `parent`：父组件指针，默认 `nullptr`。

---

### `~ElaScrollPage()`

- **功能**：析构函数，清理资源。

---

## 公共方法

### `void addCentralWidget(QWidget* centralWidget, bool isWidgetResizeable = true, bool isVerticalGrabGesture = true, qreal mousePressEventDelay = 0.1)`

- **功能**：添加一个中心页面，封装在带手势和动画的滚动区域中。
- **参数**：
  - `centralWidget`：中心页面控件，不能为空。
  - `isWidgetResizeable`：是否允许自动调整大小，默认 `true`。
  - `isVerticalGrabGesture`：是否启用竖向抓取手势，默认 `true`。
  - `mousePressEventDelay`：鼠标按压事件延迟（秒），默认 `0.1`。

---

### `void setCustomWidget(QWidget* widget)`

- **功能**：设置自定义控件，插入主布局中面包屑与中心区域之间。
- **参数**：
  - `widget`：自定义控件指针，不能为空且不能为自身。

---

### `QWidget* getCustomWidget() const`

- **功能**：获取当前设置的自定义控件。
- **返回**：自定义控件指针，若无则返回 `nullptr`。

---

### `void navigation(int widgetIndex, bool isLogRoute = true)`

- **功能**：导航切换到指定索引的中心页面，并可选择是否记录导航路由。
- **参数**：
  - `widgetIndex`：中心页面索引。
  - `isLogRoute`：是否记录路由信息，默认 `true`。

---

### `void setPageTitleSpacing(int spacing)`

- **功能**：设置面包屑导航栏左侧的间距。
- **参数**：
  - `spacing`：间距大小（像素）。

---

### `int getPageTitleSpacing() const`

- **功能**：获取当前面包屑导航栏左侧的间距。
- **返回**：间距大小（像素）。

---

### `void setTitleVisible(bool isVisible)`

- **功能**：设置面包屑导航栏的可见性。
- **参数**：
  - `isVisible`：是否显示，`true` 为显示，`false` 为隐藏。

---

## 信号

### `void pCustomWidgetChanged()`

- **说明**：当自定义控件被设置或替换时发出。

---

## 内部机制及路由

- 使用 `ElaBreadcrumbBar` 管理面包屑导航，支持点击返回。
- 通过 `QStackedWidget` 管理多个中心页面。
- 使用 `ElaScrollArea` 和 `ElaScrollBar` 实现带动画和手势支持的滚动功能。
- 结合 `ElaNavigationRouter` 实现导航路由记录与回调。

---

## 依赖组件

- **ElaBreadcrumbBar**：面包屑导航栏。
- **ElaScrollArea**：支持动画和手势的滚动区域。
- **ElaScrollBar**：自定义滚动条控件。
- **ElaNavigationRouter**：导航路由管理器，用于处理页面切换路由事件。

---

# 使用示例（简略）

```cpp
ElaScrollPage* page = new ElaScrollPage(this);
QWidget* widget1 = new QWidget();
widget1->setWindowTitle("首页");
page->addCentralWidget(widget1);

QWidget* customWidget = new QWidget();
page->setCustomWidget(customWidget);

page->navigation(0);
page->setPageTitleSpacing(10);
page->setTitleVisible(true);
