#include "Button.h"
#include <stdio.h>

QPushButton* create_button(const char* name, ButtonEvent event) {
    int padding = 3;
    QPushButton* button = new QPushButton("");
    QObject::connect(button, &QPushButton::clicked, event);
    QIcon icon = QIcon::fromTheme(name);
    QPixmap pixmap = icon.pixmap(icon.actualSize(QSize(48, 48)));
    button->setIcon(icon);
    button->setIconSize(pixmap.rect().size());
    button->setFixedSize(pixmap.rect().size().height()+padding, pixmap.rect().size().width()+padding);
    return button;
}