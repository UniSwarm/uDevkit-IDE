#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QListWidget>
#include <QStackedWidget>

#include "settingscateg.h"

class SettingsWindow : public QDialog
{
    Q_OBJECT
public:
    SettingsWindow(QWidget *parent = Q_NULLPTR);

    SettingsCateg *category(int i) const;
    SettingsCateg *addCategory(const QIcon &icon, const QString &label);
    void addCategory(SettingsCateg *category);

public slots:
    void commitChange();

protected slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);
    void buttonClick(QAbstractButton *button);

protected:
    void createWidgets();

    QListWidget *_sectionsList;
    QStackedWidget *_sectionsStack;
    QDialogButtonBox *_dialogButtonBox;
};

#endif // SETTINGSWINDOW_H
