#include "process_item.h"
#include "process_tools.h"
#include "utils.h"
#include <QDebug>

using namespace processTools;

ProcessItem::ProcessItem(QPixmap processIcon, QString processName, double processCpu, int processMemory, int processPid, QString processUser)
{
    iconPixmap = processIcon;
    name = processName;
    cpu = processCpu;
    pid = processPid;
    memory = processMemory;
    user = processUser;
    
    memoryString = Utils::convertSizeUnit(memory);

    iconSize = 24;
    
    padding = 10;
}

bool ProcessItem::sameAs(ListItem *item) 
{
    return pid == ((static_cast<ProcessItem*>(item)))->pid;
}

void ProcessItem::drawBackground(QRect rect, QPainter *painter, int index, bool isSelect) 
{
    // Init draw path.
    QPainterPath path;
    path.addRect(QRectF(rect));

    // Draw selected effect.
    if (isSelect) {
        painter->setOpacity(0.8);
        painter->fillPath(path, QColor("#006BBA"));
    }
    // Draw background effect.
    else {
        // Use different opacity with item index.
        if (index % 2 == 0) {
            painter->setOpacity(0.1);
        } else {
            painter->setOpacity(0.2);
        }
        
        painter->fillPath(path, QColor("#000000"));
    }
}

void ProcessItem::drawForeground(QRect rect, QPainter *painter, int column, int, bool isSelect) 
{
    // Init opacity and font size.
    painter->setOpacity(1);
    Utils::setFontSize(*painter, 11);
    
    // Set font color with selected status.
    if (isSelect) {
        painter->setPen(QPen(QColor("#ffffff")));
    } else {
        painter->setPen(QPen(QColor("#666666")));
    }

    // Draw icon and process name.
    if (column == 0) {
        painter->drawPixmap(QRect(rect.x() + padding, rect.y() + (rect.height() - iconSize) / 2, iconSize, iconSize), iconPixmap);

        painter->drawText(QRect(rect.x() + iconSize + padding * 2, rect.y(), rect.width() - iconSize - padding * 3, rect.height()), Qt::AlignLeft | Qt::AlignVCenter, name);
    }
    // Draw CPU.
    else if (column == 1) {
        painter->drawText(QRect(rect.x(), rect.y(), rect.width() - padding, rect.height()), Qt::AlignRight | Qt::AlignVCenter, QString("%1%").arg(QString::number(cpu, 'f', 1)));
    }
    // Draw memory.
    else if (column == 2) {
        painter->drawText(QRect(rect.x(), rect.y(), rect.width() - padding, rect.height()), Qt::AlignRight | Qt::AlignVCenter, memoryString);
    }
    // Draw pid.
    else if (column == 3) {
        painter->drawText(QRect(rect.x(), rect.y(), rect.width() - padding, rect.height()), Qt::AlignRight | Qt::AlignVCenter, QString("%1").arg(pid));
    }
}

QString ProcessItem::getName() const 
{
    return name;
}

double ProcessItem::getCPU() const 
{
    return cpu;
}

int ProcessItem::getMemory() const 
{
    return memory;
}

int ProcessItem::getPid() const 
{
    return pid;
}
    
bool ProcessItem::sortByName(const ListItem *item1, const ListItem *item2, bool descendingSort) 
{
    // Init.
    QString name1 = (static_cast<const ProcessItem*>(item1))->getName();
    QString name2 = (static_cast<const ProcessItem*>(item2))->getName();
    bool sortOrder;

    // Sort item with cpu if name is same.
    if (name1 == name2) {
        int cpu1 = static_cast<const ProcessItem*>(item1)->getCPU();
        int cpu2 = (static_cast<const ProcessItem*>(item2))->getCPU();
        
        sortOrder = cpu1 > cpu2;
    }
    // Otherwise sort by name.
    else {
        sortOrder = name1 > name2;
    }
    
    return descendingSort ? sortOrder : !sortOrder;
}

bool ProcessItem::sortByCPU(const ListItem *item1, const ListItem *item2, bool descendingSort) 
{
    // Init.
    int cpu1 = (static_cast<const ProcessItem*>(item1))->getCPU();
    int cpu2 = (static_cast<const ProcessItem*>(item2))->getCPU();
    bool sortOrder;
    
    // Sort item with memory if cpu is same.
    if (cpu1 == cpu2) {
        int memory1 = static_cast<const ProcessItem*>(item1)->getMemory();
        int memory2 = (static_cast<const ProcessItem*>(item2))->getMemory();
        
        sortOrder = memory1 > memory2;
    }
    // Otherwise sort by cpu.
    else {
        sortOrder = cpu1 > cpu2;
    }
    
    return descendingSort ? sortOrder : !sortOrder;
}

bool ProcessItem::sortByMemory(const ListItem *item1, const ListItem *item2, bool descendingSort) 
{
    // Init.
    int memory1 = (static_cast<const ProcessItem*>(item1))->getMemory();
    int memory2 = (static_cast<const ProcessItem*>(item2))->getMemory();
    bool sortOrder;
    
    // Sort item with cpu if memory is same.
    if (memory1 == memory2) {
        int cpu1 = static_cast<const ProcessItem*>(item1)->getCPU();
        int cpu2 = (static_cast<const ProcessItem*>(item2))->getCPU();
        
        sortOrder = cpu1 > cpu2;
    }
    // Otherwise sort by memory.
    else {
        sortOrder = memory1 > memory2;
    }
    
    return descendingSort ? sortOrder : !sortOrder;
}

bool ProcessItem::sortByPid(const ListItem *item1, const ListItem *item2, bool descendingSort) 
{
    bool sortOrder = (static_cast<const ProcessItem*>(item1))->getPid() > (static_cast<const ProcessItem*>(item2))->getPid();
    
    return descendingSort ? sortOrder : !sortOrder;
}


