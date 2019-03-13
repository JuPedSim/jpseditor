#include "sourcewidget.h"
#include "ui_sourcewidget.h"

#include <QDebug>

SourceWidget::SourceWidget(QWidget *parent, jpsGraphicsView *gview, jpsDatamanager *dmanager) :
    QWidget(parent),
    ui(new Ui::SourceWidget)
{
    ui->setupUi(this);

    datamanager=dmanager;
    graphview=gview;

    showSource();
    connect(graphview, SIGNAL(newSourceAdd()), this, SLOT(showSource()));
}

SourceWidget::~SourceWidget()
{
    delete ui;
}


void SourceWidget::showSource()
{
    qDebug(">> Enter SourceWidget::showSource");
    // update source list
    ui->SourceListWidget->clear();

    QList<JPSSource *> sourcelist = graphview->getSourceVector();
    for (int i=0; i<sourcelist.size(); i++)
    {
        ui->SourceListWidget->addItem(sourcelist[i]->getCaption());
    }


    qDebug("<< Leave SourceWidget::showSource");
}
