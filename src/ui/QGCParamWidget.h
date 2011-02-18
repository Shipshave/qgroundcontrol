/*=====================================================================

QGroundControl Open Source Ground Control Station

(c) 2009, 2010 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>

This file is part of the QGROUNDCONTROL project

    QGROUNDCONTROL is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    QGROUNDCONTROL is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with QGROUNDCONTROL. If not, see <http://www.gnu.org/licenses/>.

======================================================================*/

/**
 * @file
 *   @brief Declaration of class QGCParamWidget
 *   @author Lorenz Meier <mail@qgroundcontrol.org>
 */

#ifndef QGCPARAMWIDGET_H
#define QGCPARAMWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMap>
#include <QLabel>
#include <QTimer>

#include "UASInterface.h"

/**
 * @brief Widget to read/set onboard parameters
 */
class QGCParamWidget : public QWidget
{
Q_OBJECT
public:
    explicit QGCParamWidget(UASInterface* uas, QWidget *parent = 0);
    /** @brief Get the UAS of this widget */
    UASInterface* getUAS();
signals:
    /** @brief A parameter was changed in the widget, NOT onboard */
    void parameterChanged(int component, QString parametername, float value);
    /** @brief Request a single parameter */
    void requestParameter(int component, int parameter);
public slots:
    /** @brief Add a component to the list */
    void addComponent(int uas, int component, QString componentName);
    /** @brief Add a parameter to the list with retransmission / safety checks */
    void addParameter(int uas, int component, int paramCount, int paramId, QString parameterName, float value);
    /** @brief Add a parameter to the list */
    void addParameter(int uas, int component, QString parameterName, float value);
    /** @brief Request list of parameters from MAV */
    void requestParameterList();
    /** @brief Set one parameter, changes value in RAM of MAV */
    void setParameter(int component, QString parameterName, float value);
    /** @brief Set all parameters, changes the value in RAM of MAV */
    void setParameters();
    /** @brief Write the current parameters to permanent storage (EEPROM/HDD) */
    void writeParameters();
    /** @brief Read the parameters from permanent storage to RAM */
    void readParameters();
    /** @brief Clear the parameter list */
    void clear();
    /** @brief Update when user changes parameters */
    void parameterItemChanged(QTreeWidgetItem* prev, int column);

    /** @brief Store parameters to a file */
    void saveParameters();
    /** @brief Load parameters from a file */
    void loadParameters();

    /** @brief Check for missing parameters */
    void retransmissionGuardTick();

protected:
    UASInterface* mav;   ///< The MAV this widget is controlling
    QTreeWidget* tree;   ///< The parameter tree
    QLabel* statusLabel; ///< Parameter transmission label
    QMap<int, QTreeWidgetItem*>* components; ///< The list of components
    QMap<int, QMap<QString, QTreeWidgetItem*>* > paramGroups; ///< Parameter groups
    QMap<int, QMap<QString, float>* > changedValues; ///< Changed values
    QMap<int, QMap<QString, float>* > parameters; ///< All parameters
    QVector<bool> received; ///< Successfully received parameters
    QMap<int, QList<int>* > transmissionMissingPackets; ///< Missing packets
    QMap<int, QMap<QString, float>* > transmissionMissingWriteAckPackets; ///< Missing write ACK packets
    bool transmissionListMode;       ///< Currently requesting list
    QMap<int, bool> transmissionListSizeKnown;  ///< List size initialized?
    bool transmissionActive;         ///< Missing packets, working on list?
    quint64 transmissionTimeout;     ///< Timeout
    QTimer retransmissionTimer;      ///< Timer handling parameter retransmission
    int retransmissionTimeout; ///< Retransmission request timeout, in milliseconds
    int rewriteTimeout; ///< Write request timeout, in milliseconds
    int retransmissionBurstRequestSize; ///< Number of packets requested for retransmission per burst

    /** @brief Activate / deactivate parameter retransmission */
    void setRetransmissionGuardEnabled(bool enabled);
    /** @brief Load  settings */
    void loadSettings();
};

#endif // QGCPARAMWIDGET_H
