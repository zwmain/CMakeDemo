#include <QWidget>
#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QWidget w;
    w.setWindowTitle("这里是Qt");
    w.show();
    return app.exec();
}
