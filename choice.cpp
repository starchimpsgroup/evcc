#include "choice.h"
#include "ui_choice.h"

Choice::Choice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Choice)
{
    ui->setupUi(this);
}

Choice::~Choice()
{
    delete ui;
}
