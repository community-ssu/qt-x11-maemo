#include <QtGui>
#include <QtGui/QX11Info>
#include <X11/Xlib.h>

class Panel : public QDialog {
public:
    Panel(Window parent, bool user)
        : QDialog(0), m_parent(parent)
    {
        QString text = tr("This is a control panel\nwritten with Qt.");
        if (user)
            text.append(tr("\n(Panel was opened by the user)"));
        QLabel *l = new QLabel(text);
        QDialogButtonBox *b = new QDialogButtonBox();
        b->setOrientation(Qt::Vertical);
        b->setStandardButtons(QDialogButtonBox::Save | QDialogButtonBox::Cancel);
        QHBoxLayout *layout = new QHBoxLayout(this);
        layout->addWidget(l, 1);
        layout->addWidget(b, 0);

        setWindowTitle(tr("Qt Control Panel"));

        if (m_parent != None)
            setAttribute(Qt::WA_X11BypassTransientForHint);
    }

protected:
    void showEvent(QShowEvent *se)
    {
        if (m_parent != None)
            XSetTransientForHint(QX11Info::display(), winId(), m_parent);
        QDialog::showEvent(se);
    }
private:
    Window m_parent;
};


int main(int argc, char **argv)
{
    QApplication a(argc, argv);

    bool user = false;
    Window parent = None;

    QStringList args = a.arguments();
    for (int i = 1; i < args.count(); i++) {
        QString s = args[i];

        if (s == QLatin1String("--user"))
            user = true;
        if (s == QLatin1String("--parent"))
            parent = args[++i].toInt(0, 0);
    }

    Panel p(parent, user);
    return p.exec();
}
