#pragma once

#include <QGraphicsItem>

class Node;

class Edge : public QGraphicsItem {
    Node *source, *dest;
    QPointF sourcePoint;
    QPointF destPoint;
    qreal arrowSize;
    QString name;

  public:
    Edge(Node * sourceNode, Node * destNode, QString _name);

    Node * sourceNode() const;
    Node * destNode() const;

    void adjust();

    enum { Type = UserType + 2 };
    int type() const override { return Type; }

  protected:
    QRectF boundingRect() const override;

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option,
               QWidget * widget) override;
};
