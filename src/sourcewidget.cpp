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

    connect(graphview, SIGNAL(newSourceAdd()), this, SLOT(showSource()));
    connect(graphview, SIGNAL(newSourceAdd()), this, SLOT(showSourceInformation()));
    connect(ui->SourceListWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(showSourceInformation()));
    connect(ui->SourceListWidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(
            showSourceInformation()));
    connect(ui->applyButton,SIGNAL(clicked(bool)),this,SLOT(applySourceInformation()));

    showSource();
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

void SourceWidget::showSourceInformation()
{
    qDebug(">> Enter showSourceInfomation::showSourceInformation");
    if(ui->SourceListWidget->currentItem() != nullptr)
    {
        int crow = ui->SourceListWidget->currentRow();
        auto source = graphview->getSourceVector()[crow];

        ui->IDlineEdit->setText(QString::number(source->getId()));
        ui->agents_maxLineEdit->setText(source->getAgents_max());
        ui->captionLineEdit->setText(source->getCaption());
        ui->frequencyLineEdit->setText(source->getFrequency());
        ui->group_IDLineEdit->setText(source->getGroup_id());
        ui->N_createLineEdit->setText(source->getN_create());
        ui->percentLineEdit->setText(source->getPercent());
        ui->rateLineEdit->setText(source->getRate());
        ui->startYLineEdit->setText(source->getStartY());
        ui->startXLineEdit->setText(source->getStartX());
        ui->timeLineEdit->setText(source->getTime());
        ui->time_minLineEdit->setText(source->getTime_min());
        ui->time_maxLineEdit->setText(source->getTime_max());
        ui->greedyLineEdit->setText(source->getGreedy());
        ui->X_MinLineEdit->setText(QString::number(source->getX_min()));
        ui->X_MaxllneEdit->setText(QString::number(source->getX_max()));
        ui->Y_MinLineEdit->setText(QString::number(source->getY_min()));
        ui->Y_MaxLineEdit->setText(QString::number(source->getY_max()));

        if(source->isBeSaved())
        {
            ui->isSaveButton->setChecked(true);
        } else
        {
            ui->isSaveButton->setChecked(false);
        }
    }
    qDebug("<< Leave showSourceInfomation::showSourceInformation");
}

void SourceWidget::applySourceInformation()
{
    qDebug(">> Enter SourceWidget::applySourceInformation");
    int crow = ui->SourceListWidget->currentRow();
    auto source = graphview->getSourceVector()[crow];
    if(ui->SourceListWidget->currentItem() != nullptr)
    {
        source->setId(ui->IDlineEdit->text().toInt());
        source->setAgents_max(ui->agents_maxLineEdit->text());
        source->setCaption(ui->captionLineEdit->text());
        source->setFrequency(ui->frequencyLineEdit->text());
        source->setGroup_id(ui->group_IDLineEdit->text());
        source->setN_create(ui->N_createLineEdit->text());
        source->setPercent(ui->percentLineEdit->text());
        source->setRate(ui->rateLineEdit->text());
        source->setStartX(ui->startXLineEdit->text());
        source->setStartY(ui->startYLineEdit->text());
        source->setTime(ui->timeLineEdit->text());
        source->setTime_max(ui->time_maxLineEdit->text());
        source->setTime_min(ui->time_minLineEdit->text());
        source->setX_min(ui->X_MinLineEdit->text().toFloat());
        source->setX_max(ui->X_MaxllneEdit->text().toFloat());
        source->setY_min(ui->Y_MinLineEdit->text().toFloat());
        source->setY_max(ui->Y_MaxLineEdit->text().toFloat());

        if(ui->isSaveButton->isChecked())
        {
            source->setBeSaved(true);
        } else
        {
            source->setBeSaved(false);
        }
    }

    qDebug("<< Leave SourceWidget::applySourceInformation");
}
