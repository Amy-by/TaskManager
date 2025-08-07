#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>  // 用于弹窗提示
#include <QFile>        // 用于文件操作
#include <QTextStream>  // 用于文件读写流
#include <QDir>         // 用于路径处理
#include <QDate>        // 用于日期处理

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 设置窗口标题
    setWindowTitle("个人任务管理工具");

    // 初始化优先级下拉框
    //ui->priorityCombo->addItem("高");
    //ui->priorityCombo->addItem("中");
    //ui->priorityCombo->addItem("低");

    // 加载本地保存的任务
    loadTasks();

    // 绑定"添加任务"按钮的点击事件到addTask函数
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::addTask);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 实现添加任务功能
void MainWindow::addTask()
{
    // 获取输入的任务内容
    QString taskContent = ui->taskInput->text().trimmed();
    // 获取选择的优先级
    QString priority = ui->priorityCombo->currentText();
    // 获取当前日期作为截止日期（实际项目可改为用户输入）
    QString deadline = QDate::currentDate().toString("yyyy-MM-dd");

    // 校验任务内容是否为空
    if (taskContent.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "任务内容不能为空！");
        return;
    }

    // 创建任务对象并添加到列表
    Task newTask;
    newTask.content = taskContent;
    newTask.priority = priority;
    newTask.deadline = deadline;
    taskList.append(newTask);

    // 在界面上显示任务（格式化显示）
    QString displayText = QString("[%1] %2 （截止：%3）")
                            .arg(priority)
                            .arg(taskContent)
                            .arg(deadline);
    ui->taskList->addItem(displayText);

    // 清空输入框
    ui->taskInput->clear();

    // 保存任务到本地文件
    saveTasks();

    // 检查是否为当天任务并提醒
    if (deadline == QDate::currentDate().toString("yyyy-MM-dd")) {
        QMessageBox::information(this, "任务提醒", "已添加当天截止的任务！");
    }
}

// 保存任务到本地文件
void MainWindow::saveTasks()
{
    // 定义文件路径（Linux: /home/用户名/tasks.txt; Windows: C:/Users/用户名/tasks.txt）
    QString filePath = QDir::homePath() + "/tasks.txt";

    QFile file(filePath);
    // 以只写、文本模式打开文件（若文件不存在则创建）
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "保存失败", "无法打开文件进行写入！");
        return;
    }

    QTextStream out(&file);
    // 按格式写入所有任务（内容|优先级|截止日期）
    for (const Task& task : taskList) {
        out << task.content << "|" << task.priority << "|" << task.deadline << "\n";
    }

    file.close();  // 关闭文件
}

// 从本地文件加载任务
void MainWindow::loadTasks()
{
    QString filePath = QDir::homePath() + "/tasks.txt";
    QFile file(filePath);

    // 以只读、文本模式打开文件（若文件不存在则直接返回）
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;  // 首次运行无文件时正常，不弹窗提示
    }

    QTextStream in(&file);
    // 逐行读取文件内容
    while (!in.atEnd()) {
        QString line = in.readLine();
        // 按"|"分割字符串（解析任务数据）
        QStringList parts = line.split("|");
        if (parts.size() == 3) {  // 确保数据格式正确
            Task loadedTask;
            loadedTask.content = parts[0];
            loadedTask.priority = parts[1];
            loadedTask.deadline = parts[2];

            taskList.append(loadedTask);

            // 在界面上显示加载的任务
            QString displayText = QString("[%1] %2 （截止：%3）")
                                    .arg(loadedTask.priority)
                                    .arg(loadedTask.content)
                                    .arg(loadedTask.deadline);
            ui->taskList->addItem(displayText);
        }
    }

    file.close();  // 关闭文件
}
