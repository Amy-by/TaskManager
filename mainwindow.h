#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QList>  // 用于存储任务列表
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 点击"添加任务"按钮时触发的槽函数
    void addTask();

private:
    Ui::MainWindow *ui;

    // 任务结构体：存储单个任务的信息
    struct Task {
        QString content;   // 任务内容
        QString priority;  // 优先级（高/中/低）
        QString deadline;  // 截止日期（格式：yyyy-MM-dd）
    };

    QList<Task> taskList;  // 存储所有任务的列表

    // 函数声明
    void saveTasks();      // 将任务列表保存到本地文件
    void loadTasks();      // 从本地文件加载任务列表
};
#endif // MAINWINDOW_H
