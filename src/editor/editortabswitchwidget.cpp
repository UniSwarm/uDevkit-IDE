#include "editortabswitchwidget.h"

#include <QDebug>
#include <QEvent>
#include <QKeyEvent>

EditorTabSwitchWidget::EditorTabSwitchWidget(QWidget *parent)
    : QListWidget(parent)
{
    setSelectionMode(SingleSelection);
    connect(this, &QListWidget::currentRowChanged, this, &EditorTabSwitchWidget::updateId);
    connect(this, &QListWidget::itemClicked, this, &EditorTabSwitchWidget::clickItem);
}

int EditorTabSwitchWidget::id() const
{
    return _id;
}

void EditorTabSwitchWidget::setId(int id)
{
    _id = id;
    setCurrentRow(_id);
}

void EditorTabSwitchWidget::updateId(int row)
{
    if (_id != -1)
        _id = row;
}

void EditorTabSwitchWidget::clickItem(QListWidgetItem *item)
{
    if (item)
    {
        _id = indexFromItem(item).row();
        hide();
    }
}

bool EditorTabSwitchWidget::event(QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Tab || keyEvent->key() == Qt::Key_Backtab)
        {
            if (keyEvent->modifiers() & Qt::ShiftModifier)
            {
                _id--;
                if (_id < 0)
                    _id = count() - 1;
            }
            else
            {
                _id++;
                if (_id >= count())
                    _id = 0;
            }
            setCurrentRow(_id);
            keyEvent->ignore();
            return false;
        }
        else if (keyEvent->key() == Qt::Key_Escape)
        {
            _id = 0;
            setCurrentRow(_id);
            hide();
        }
        return QListWidget::event(event);
    }
    return QListWidget::event(event);
}
