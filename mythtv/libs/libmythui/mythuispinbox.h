#ifndef MYTHUISPINBOX_H_
#define MYTHUISPINBOX_H_

#include "mythuibuttonlist.h"

/** \class MythUISpinBox
 *
 * \brief A widget for offering a range of numerical values where only the
 *        the bounding values and interval are known.
 *
 * Where a list of specific values is required instead, then use
 * MythUIButtonList instead.
 *
 */
class MPUBLIC MythUISpinBox : public MythUIButtonList
{
    Q_OBJECT
  public:
    MythUISpinBox(MythUIType *parent, const QString &name);
    ~MythUISpinBox();

    void SetRange(int low, int high, int step);

    void SetValue(int val) { SetValueByData(val); }
    void SetValue(const QString &val) { SetValueByData(val.toInt()); }
    QString GetValue(void) const { return GetDataValue().toString(); }
    int GetIntValue(void) const { return GetDataValue().toInt(); }

  protected:
    virtual bool ParseElement(QDomElement &element);
    virtual void CopyFrom(MythUIType *base);
    virtual void CreateCopy(MythUIType *parent);

    bool m_hasTemplate;
    QString m_negativeTemplate;
    QString m_zeroTemplate;
    QString m_positiveTemplate;
};

#endif
