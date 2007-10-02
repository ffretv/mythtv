#ifndef CHANNELBASE_H
#define CHANNELBASE_H

// Qt headers
#include <qmap.h>
#include <qstringlist.h>

// MythTV headers
#include "channelutil.h"
#include "tv.h"

class TVRec;

class InputBase
{
  public:
    InputBase() :
        name(QString::null),            startChanNum(QString::null),
        tuneToChannel(QString::null),   externalChanger(QString::null),
        sourceid(0),                    cardid(0),
        inputNumV4L(-1),
        videoModeV4L1(0),               videoModeV4L2(0) {}

    InputBase(QString _name,            QString _startChanNum,
              QString _tuneToChannel,   QString _externalChanger,
              uint    _sourceid,        uint    _cardid,
              const DBChanList &_channels) :
        name(_name),                    startChanNum(_startChanNum),
        tuneToChannel(_tuneToChannel),  externalChanger(_externalChanger),
        sourceid(_sourceid),            cardid(_cardid),
        channels(_channels),
        inputNumV4L(-1),
        videoModeV4L1(0),               videoModeV4L2(0) {}

    virtual ~InputBase() {}

  public:
    QString name;            // input name
    QString startChanNum;    // channel to start on
    QString tuneToChannel;   // for using a cable box & S-Video/Composite
    QString externalChanger; // for using a cable box...
    uint    sourceid;        // associated channel listings source
    uint    cardid;          // input card id
    DBChanList channels;
    int     inputNumV4L;
    int     videoModeV4L1;
    int     videoModeV4L2;
};
typedef QMap<uint, InputBase*> InputMap;

/** \class ChannelBase
 *  \brief Abstract class providing a generic interface to tuning hardware.
 *
 *   This class abstracts channel implementations for analog TV, ATSC, DVB, etc.
 *   Also implements many generic functions needed by most derived classes.
 *   It is responsible for tuning, i.e. switching channels.
 */

class ChannelBase
{
 public:
    ChannelBase(TVRec *parent);
    virtual ~ChannelBase();

    // Methods that must be implemented.
    /// \brief Opens the channel changing hardware for use.
    virtual bool Open(void) = 0;
    /// \brief Closes the channel changing hardware to use.
    virtual void Close(void) = 0;
    virtual bool SetChannelByString(const QString &chan) = 0;
    /// \brief Reports whether channel is already open
    virtual bool IsOpen(void) const = 0;

    // Methods that one might want to specialize
    /// \brief Sets file descriptor.
    virtual void SetFd(int fd) { (void)fd; };
    /// \brief Returns file descriptor, -1 if it does not exist.
    virtual int GetFd(void) const { return -1; };

    // Gets
    virtual uint GetNextChannel(uint chanid, int direction) const;
    virtual uint GetNextChannel(const QString &channum, int direction) const;
    virtual int GetInputByName(const QString &input) const;
    virtual QString GetInputByNum(int capchannel) const;
    virtual QString GetCurrentName(void) const
        { return curchannelname; }
    virtual int GetChanID(void) const;
    virtual int GetCurrentInputNum(void) const
        { return currentInputID; }
    virtual QString GetCurrentInput(void) const
        { return inputs[GetCurrentInputNum()]->name; }
    virtual int GetNextInputNum(void) const;
    virtual QString GetNextInput(void) const
        { return inputs[GetNextInputNum()]->name; }
    virtual QString GetNextInputStartChan(void)
        { return inputs[GetNextInputNum()]->startChanNum; }
    virtual uint GetCurrentSourceID(void) const
        { return inputs[GetCurrentInputNum()]->sourceid; }
    virtual uint GetInputCardID(int inputNum) const;
    virtual DBChanList GetChannels(int inputNum) const;
    virtual DBChanList GetChannels(const QString &inputname) const;
    virtual QStringList GetConnectedInputs(void) const;
    /// \brief Returns true iff commercial detection is not required
    //         on current channel, for BBC, CBC, etc.
    bool IsCommercialFree(void) const { return commfree; }
    /// \brief Returns String representing device, useful for debugging
    virtual QString GetDevice(void) const { return ""; }

    // Sets
    virtual void Renumber(uint srcid, const QString &oldChanNum,
                          const QString &newChanNum);

    // Channel setting convenience method
    virtual bool SetChannelByDirection(ChannelChangeDirection);

    // Input toggling convenience methods
    virtual bool SwitchToInput(const QString &input);
    virtual bool SwitchToInput(const QString &input, const QString &chan);

    virtual bool InitializeInputs(void);

    // Misc. Commands
    virtual bool Retune(void) { return false; }

    /// Saves current channel as the default channel for the current input.
    virtual void StoreInputChannels(void)
        { StoreInputChannels(inputs);
          StoreDefaultInput(GetCardID(), GetCurrentInput()); }

    // Picture attribute settings
    virtual bool InitPictureAttributes(void) { return false; }
    virtual int  GetPictureAttribute(PictureAttribute) const { return -1; }
    virtual int  ChangePictureAttribute(
        PictureAdjustType, PictureAttribute, bool up) { return -1; }

    bool CheckChannel(const QString &channum, QString& inputName) const;

    // \brief Set cardid for scanning
    void SetCardID(uint _cardid) { cardid = _cardid; }

  protected:
    /// \brief Switches to another input on hardware, 
    ///        and sets the channel is setstarting is true.
    virtual bool SwitchToInput(int newcapchannel, bool setstarting) = 0;

    virtual int GetCardID(void) const;
    virtual bool ChangeExternalChannel(const QString &newchan);
    static void StoreInputChannels(const InputMap&);
    static void StoreDefaultInput(uint cardid, const QString &input);

    TVRec   *pParent;
    QString  curchannelname;
    int      currentInputID;
    bool     commfree;
    uint     cardid;
    InputMap inputs;
    DBChanList allchannels; ///< channels across all inputs
};

#endif

