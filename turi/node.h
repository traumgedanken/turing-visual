#pragma once

#include <QGraphicsItem>
#include <QList>

class Edge;
class GraphWidget;
class QGraphicsSceneMouseEvent;

class Node : public QGraphicsItem {
    QList<Edge *> edgeList;
    QPointF newPos;
    GraphWidget * graph;
    QString name;

  public:
    Node(GraphWidget * graphWidget, QString name);

    QString getName();

    void addEdge(Edge * edge);

    QList<Edge *> edges() const;

    enum { Type = UserType + 1 };
    int type() const override { return Type; }

    void calculateForces();

    bool advance();

    QRectF boundingRect() const override;

    QPainterPath shape() const override;

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option,
               QWidget * widget) override;

  protected:
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant & value) override;

    void mousePressEvent(QGraphicsSceneMouseEvent * event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;
};
