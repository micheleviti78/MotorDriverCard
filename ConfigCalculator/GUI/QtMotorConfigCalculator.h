#ifndef QT_MOTOR_CONFIG_CALCULATOR_H
#define QT_MOTOR_CONFIG_CALCULATOR_H

#include "MotorConfigWidget.h"
#include <QTabWidget>
#include <QPushButton>

/** A widget containing two MotorConfigWidget in a TabWidget,
 *  and a write button which write the config to an XML file\
 */
class QtMotorConfigCalculator: public QWidget{
   Q_OBJECT

 public:
  /** The constructor expects no parameters except for the 
   *  parent widget.
   */
   QtMotorConfigCalculator(QWidget * parent_ = NULL);

 private:
   QVBoxLayout *verticalLayout;
   QTabWidget *motorTabWidget;
   QPushButton *writeButton;

   std::vector<MotorConfigWidget *> motorConfigWidgets;

 private slots:
   /** Get the file name via dialogue and perform the writing.
    */
   void write();

};

#endif// QT_MOTOR_CONFIG_CALCULATOR_H