/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "graphwidget.h"
#include "edge.h"
#include "node.h"
#include <QDebug>
#include <math.h>

#include <QKeyEvent>
//#include <QRandomGenerator>

GraphWidget::GraphWidget(QWidget * parent, TuriProgram * program)
    : QGraphicsView(parent), timerId(0) {
    QGraphicsScene * scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-200, -200, 400, 400);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8), qreal(0.8));
    setMinimumSize(400, 400);
    setWindowTitle(tr("Elastic Nodes"));

    if (program == nullptr) return;
    QString endStateName = "!";
    centerNode = new Node(this, endStateName);
    states.push_back(centerNode);
    scene->addItem(centerNode);
    for (int i = 0; i < program->count(); i++) {
        QString stateName = program->getCommand(i)->getCurrentState();
        if (getNode(stateName) == nullptr) {
            Node * newNode = new Node(this, stateName);
            states.push_back(newNode);
            scene->addItem(newNode);
        }
    }

    for (int i = 0; i < program->count(); i++) {
        TuriCommand * currentCommand = program->getCommand(i);
        QString currentState = currentCommand->getCurrentState();
        QString nextState = currentCommand->getNextState();
        Node * start = getNode(currentState);
        Node * end = getNode(nextState);
        scene->addItem(
            new Edge(start, end, currentCommand->toArrowDescription()));
    }

    int n = std::sqrt(states.length());
    int step = 100 / (n + 1);
    int row = 0;
    int column = 0;
    for (auto & node : states) {
        node->setPos(-50 + column * step, -50 + row * step);
        column++;
        if (column == n) {
            column = 0;
            row++;
        }
    }
}

Node * GraphWidget::getNode(QString stateName) {
    for (auto & node : states) {
        if (node->getName() == stateName) return node;
    }
    return nullptr;
}

void GraphWidget::itemMoved() {
    if (!timerId) timerId = startTimer(1000 / 25);
}

void GraphWidget::keyPressEvent(QKeyEvent * event) {
    switch (event->key()) {
    case Qt::Key_Up: centerNode->moveBy(0, -20); break;
    case Qt::Key_Down: centerNode->moveBy(0, 20); break;
    case Qt::Key_Left: centerNode->moveBy(-20, 0); break;
    case Qt::Key_Right: centerNode->moveBy(20, 0); break;
    case Qt::Key_Plus: zoomIn(); break;
    case Qt::Key_Minus: zoomOut(); break;
    case Qt::Key_Space:
    case Qt::Key_Enter: shuffle(); break;
    default: QGraphicsView::keyPressEvent(event);
    }
}

void GraphWidget::timerEvent(QTimerEvent * event) {
    Q_UNUSED(event);

    QList<Node *> nodes;
    foreach (QGraphicsItem * item, scene()->items()) {
        if (Node * node = qgraphicsitem_cast<Node *>(item)) nodes << node;
    }

    foreach (Node * node, nodes)
        node->calculateForces();

    bool itemsMoved = false;
    foreach (Node * node, nodes) {
        if (node->advance()) itemsMoved = true;
    }

    if (!itemsMoved) {
        killTimer(timerId);
        timerId = 0;
    }
}

#if QT_CONFIG(wheelevent)
void GraphWidget::wheelEvent(QWheelEvent * event) {
    scaleView(pow((double)2, -event->delta() / 240.0));
}
#endif

void GraphWidget::drawBackground(QPainter * painter, const QRectF & rect) {
    Q_UNUSED(rect);

    // Shadow
    QRectF sceneRect = this->sceneRect();
    QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5,
                       sceneRect.height());
    QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(),
                        sceneRect.width(), 5);
    if (rightShadow.intersects(rect) || rightShadow.contains(rect))
        painter->fillRect(rightShadow, Qt::darkGray);
    if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
        painter->fillRect(bottomShadow, Qt::darkGray);

    // Fill
    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::white);
    painter->fillRect(rect.intersected(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);
}

void GraphWidget::scaleView(qreal scaleFactor) {
    qreal factor = transform()
                       .scale(scaleFactor, scaleFactor)
                       .mapRect(QRectF(0, 0, 1, 1))
                       .width();
    if (factor < 0.07 || factor > 100) return;

    scale(scaleFactor, scaleFactor);
}

void GraphWidget::shuffle() {
    int i = 0;
    foreach (QGraphicsItem * item, scene()->items()) {
        if (qgraphicsitem_cast<Node *>(item))
            item->setPos(-150 + i * 20, -150 + i * 20);
        i++;
    }
}

void GraphWidget::zoomIn() { scaleView(qreal(1.2)); }

void GraphWidget::zoomOut() { scaleView(1 / qreal(1.2)); }
