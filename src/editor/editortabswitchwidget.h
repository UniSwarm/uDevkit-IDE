#ifndef EDITORTABSWITCHWIDGET_H
#define EDITORTABSWITCHWIDGET_H

#include <QListWidget>

class EditorTabSwitchWidget : public QListWidget
{
    Q_OBJECT
public:
    EditorTabSwitchWidget(QWidget *parent);

    int id() const;
    void setId(int id);

protected slots:
    void updateId(int row);
    void clickItem(QListWidgetItem *item);

    // QObject interface
public:
    bool event(QEvent *event) override;

protected:
    int _id;
};

#endif // EDITORTABSWITCHWIDGET_H
