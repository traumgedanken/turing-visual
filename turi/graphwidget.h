#pragma once

#include <QGraphicsView>
#include <turiprogram.h>

class Node;

class GraphWidget : public QGraphicsView {
    Q_OBJECT

  public:
    GraphWidget(QWidget * parent, TuriProgram * program);

    void itemMoved();

  public slots:
    void zoomIn();
    void zoomOut();

  protected:
    void keyPressEvent(QKeyEvent * event) override;
    void timerEvent(QTimerEvent * event) override;
#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent * event) override;
#endif
    void scaleView(qreal scaleFactor);

  private:
    int timerId;
    Node * centerNode;
    QVector<Node *> states;

    Node * getNode(QString stateName);
};
