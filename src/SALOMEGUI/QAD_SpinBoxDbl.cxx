//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : QAD_SpinBoxDbl.cxx
//  Author : Vadim SANDLER
//  Module : SALOME
//  $Header$

#include "QAD_SpinBoxDbl.h"
#include <qpushbutton.h>
#include <qpainter.h>
#include <qbitmap.h>
#include <qlineedit.h>
#include <qvalidator.h>
#include <qpixmapcache.h>
#include <qapplication.h>
#include <limits.h>
using namespace std;

//******************************************************************************
// QDblRangeControl class
//******************************************************************************

/*!
  Constructs a range control with min value 0.00, max value 99.99,
  line step 0.1, page step 1.00, precision 6, double precision 10e-6,
  convertion flag 'g' and initial value 0.00.
*/
QDblRangeControl::QDblRangeControl()
{
  prec     = 6;
  dblPrec  = 10e-6;
  convFlag = 'g';
  minVal   = roundPrecision(  0.00 );
  maxVal   = roundPrecision( 99.99 );
  line     = roundPrecision(  0.10 );
  page     = roundPrecision(  1.00 );
  prevVal  = roundPrecision( -0.10 );
  val      = bound( 0.00 );
}

/*!
  Constructs a range control whose value can never be smaller than
  <minValue> or greater than <maxValue>, whose line step size is
  <lineStep> and page step size is <pageStep> and whose value is
  initially <value> (which is guaranteed to be in range using bound()),
  precision is <precision>, double precision is <dblPrecision> and
  convertion flag is <cFlag>
*/
QDblRangeControl::QDblRangeControl( double minValue, double maxValue,
                                    double lineStep, double pageStep,
                                    double value, 
				    int    precision, 
				    double dblPrecision,
				    char   cFlag ) 
{
  prec     = precision;
  dblPrec  = dblPrecision;
  convFlag = cFlag;
  minVal   = roundPrecision( minValue );
  maxVal   = roundPrecision( maxValue );
  line     = roundPrecision( QABS( lineStep ) );
  page     = roundPrecision( QABS( pageStep ) );
  prevVal  = roundPrecision( minVal - 0.1 );
  val      = bound( value );
}

/*!
  Destroys the range control
*/
QDblRangeControl::~QDblRangeControl()
{
}

/*!
  Returns the current range control value.  This is guaranteed to be
  within the range [minValue(), maxValue()].
*/
inline double QDblRangeControl::value() const
{ return val; }

/*!
  Sets the range control's value to <value> and forces it to be within
  the legal range.
  Calls the virtual valueChange() function if the new value is
  different from the previous value. The old value can still be
  retrieved using prevValue().
*/
void QDblRangeControl::setValue( double value )
{
  directSetValue( value );
  if ( !equal(prevVal, val ) )
    valueChange();
}

/*!
  Equivalent to {setValue( value() + pageStep() )}.
  If the value is changed, then valueChange() is called.
*/
void QDblRangeControl::addPage()
{
  setValue( value() + pageStep() );
}

/*!
  Equivalent to {setValue( value() - pageStep() )}.
  If the value is changed, then valueChange() is called.
*/
void QDblRangeControl::subtractPage()
{
  setValue( value() - pageStep() );
}

/*!
  Equivalent to {setValue( value() + lineStep() )}.
  If the value is changed, then valueChange() is called.
*/
void QDblRangeControl::addLine()
{
  setValue( value() + lineStep() );
}

/*!
  Equivalent to {setValue( value() - lineStep() )}.
  If the value is changed, then valueChange() is called.
*/
void QDblRangeControl::subtractLine()
{
  setValue( value() - lineStep() );
}

/*!
  Returns the current minimum value of the range.
*/
inline double QDblRangeControl::minValue() const
{ return minVal; }

/*!
  Returns the current maximum value of the range.
*/
inline double QDblRangeControl::maxValue() const
{ return maxVal; }

/*!
  Sets the range control's min value to <minValue> and its max value
  to <maxValue>.

  Calls the virtual rangeChange() function if one or both of the new
  min and max values are different from the previous setting.  Calls
  the virtual valueChange() function if the current value is adjusted
  because it was outside the new range.

  If <maxValue> is smaller than <minValue>, <minValue> becomes the
  only legal value.
*/
void QDblRangeControl::setRange( double minValue, double maxValue )
{
  minValue = roundPrecision( minValue );
  maxValue = roundPrecision( maxValue );
  if ( minValue > maxValue ) {
    maxValue = minValue;
  }
  if ( equal( minValue, minVal ) && equal( maxValue, maxVal ) )
    return;
  minVal = minValue;
  maxVal = maxValue;
  double tmp = bound( val );
  rangeChange();
  if ( !equal( tmp, val ) ) {
    prevVal = val;
    val = tmp;
    valueChange();
  }
}

/*!
  Sets the current minimum value of the range to <minVal>.
  If necessary, the maxValue() is adjusted so that the range remains
  valid.
*/
void QDblRangeControl::setMinValue( double minVal )
{
  double maxVal = maxValue();
  if ( maxVal < minVal )
    maxVal = minVal;
  setRange( minVal, maxVal );
}

/*!
  Sets the current maximum value of the range to <maxVal>.
  If necessary, the minValue() is adjusted so that the range remains
  valid.
*/
void QDblRangeControl::setMaxValue( double maxVal )
{
  double minVal = minValue();
  if ( minVal > maxVal )
    minVal = maxVal;
  setRange( minVal, maxVal );
}

/*!
  Returns the current line step.
*/
inline double QDblRangeControl::lineStep() const
{ return line; }

/*!
  Returns the current page step.
*/
inline double QDblRangeControl::pageStep() const
{ return page; }

/*!
  Sets the range line step to <lineStep> and page step to <pageStep>.
  Calls the virtual stepChange() function if the new line step and/or
  page step are different from the previous settings.
*/
void QDblRangeControl::setSteps( double lineStep, double pageStep )
{
  lineStep = roundPrecision( QABS( lineStep ) );
  pageStep = roundPrecision( QABS( pageStep ) );
  if ( !equal( lineStep, line ) || !equal( pageStep, page ) ) {
    line = lineStep;
    page = pageStep;
    stepChange();
  }
}

/*!
  Returs precision ( see QString::setNum() for details )
*/
int QDblRangeControl::precision() const 
{ return prec; }

/*!
  Sets precision ( see QString::setNum() for details )
*/
void QDblRangeControl::setPrecision( int precision )
{
  if ( precision > 0 && prec != precision ) {
    prec = precision;
    setRange( minValue(), maxValue() );
    setSteps( lineStep(), pageStep() );
  }
}

/*!
  Returns double precision which is used for rounding amd comparing 
  of double numbers
*/
double QDblRangeControl::dblPrecision() const
{ return dblPrec; }

/*!
  Sets double precision which is used for rounding amd comparing 
  of double numbers
*/
void QDblRangeControl::setDblPrecision( double dblPrecision )
{
  dblPrecision = QABS( dblPrecision );
  if ( dblPrecision > 0 && dblPrecision != dblPrec ) {
    dblPrec = dblPrecision;
    setRange( minValue(), maxValue() );
    setSteps( lineStep(), pageStep() );
  }
}

/*!
  Returns convertion flag ( see QString::setNum() for details )
*/
char QDblRangeControl::convertFlag() const
{ return convFlag; }

/*!
  Sets convertion flag ( see QString::setNum() for details )
*/
void QDblRangeControl::setConvertFlag( char cFlag )
{
  if ( ( cFlag == 'f' || cFlag == 'F' || cFlag == 'e' || 
	 cFlag == 'E' || cFlag == 'g' || cFlag == 'G' ) &&
       ( cFlag != convFlag ) ) {
    convFlag = cFlag;
    setRange( minValue(), maxValue() );
    setSteps( lineStep(), pageStep() );
  }
}

/*!  
  Forces the value <v> to be within the range from minValue() to
  maxValue() inclusive, and returns the result.

  This function is provided so that you can easily force other numbers
  than value() into the allowed range.  You do not need to call it in
  order to use QDblRangeControl itself.
*/
double QDblRangeControl::bound( double v ) const
{
  if ( v < minVal )
    return minVal;
  if ( v > maxVal )
    return maxVal;
  return roundPrecision( v );
}

/*!
  Sets the range control value directly without calling valueChange().
  Forces the new value to be within the legal range.

  You will rarely have to call this function. However, if you want to
  change the range control's value inside the overloaded method
  valueChange(), setValue() would call the function valueChange()
  again. To avoid this recursion you must use directSetValue()
  instead.
*/
void QDblRangeControl::directSetValue( double value )
{
  prevVal = val;
  val = bound( value );
}

/*!
  Returns the previous value of the range control. "Previous value"
  means the value before the last change occurred. Setting a new range
  may affect the value, too, because the value is forced to be inside
  the specified range. When the range control is initially created,
  this is the same as value().

  prevValue() can be outside the current legal range if a call to
  setRange() causes the current value to change.  For example, if the
  range was [0, 1000] and the current value is 500, setRange(0, 400)
  makes value() return 400 and prevValue() return 500.
*/
inline double QDblRangeControl::prevValue() const
{ return prevVal; }

/*!
  This virtual function is called whenever the range control value
  changes.  You can reimplement it if you want to be notified when the
  value changes.  The default implementation does nothing.

  Note that this method is called after the value changed. The previous
  value can be retrieved using prevValue().
*/
void QDblRangeControl::valueChange()
{
}

/*!
  This virtual function is called whenever the range control's range
  changes.  You can reimplement it if you want to be notified when the range
  changes.  The default implementation does nothing.

  Note that this method is called after the range changed.
*/
void QDblRangeControl::rangeChange()
{
}

/*!
  This virtual function is called whenever the range control's
  line/page step settings change.  You can reimplement it if you want
  to be notified when the step changes.  The default implementation
  does nothing.

  Note that this method is called after the step settings change.
*/
void QDblRangeControl::stepChange()
{
}

/*!
  returns true if values qre equal ( with tolerance = dblPrecision() )
*/
bool QDblRangeControl::equal( double first, double second ) const
{
  return QABS( first - second ) < dblPrecision();
}

/*!
  Retuns rounded value
*/
double QDblRangeControl::roundPrecision( double value) const
{
  bool bOk;
  QString convertor;
  convertor.setNum( value, convertFlag(), precision() );
  double newValue = convertor.toDouble(&bOk);
  if ( bOk ) {
    if ( QABS( newValue ) < dblPrecision() )
      newValue = 0.0;
  }
  else {
    newValue = value;
  }
  return newValue;
}

//******************************************************************************
// QAD_SpinBox class
//******************************************************************************

/*! 
  Returns true if <str> ends by <substr> [ static ]
*/
static bool endsWith(const QString& str, const QString& substr)
{
#if QT_VERSION < 0x030000
  if ( str.length() < substr.length() )
    return false;
  return ( str.right( substr.length() ) == substr );
#else
  return str.endsWith(substr);
#endif
}

/*!
  Validator class for double value spin box
*/
class QAD_SpinBoxDblValidator: public QDoubleValidator 
{
public:
  QAD_SpinBoxDblValidator( QAD_SpinBoxDbl* sb, const char* name ) 
  : QDoubleValidator( sb, name ),
    spinBox( sb ) {}

  State validate( QString& str, int& pos ) const;

private:
  QAD_SpinBoxDbl* spinBox;
};

/*!
  Validates data entered
*/
QValidator::State QAD_SpinBoxDblValidator::validate( QString& str, int& pos ) const 
{
  QString pref = spinBox->prefix();
  QString suff = spinBox->suffix();
  uint overhead = pref.length() + suff.length();
  State state = Invalid;

  ((QDoubleValidator *) this)->setRange( spinBox->minValue(),
                                         spinBox->maxValue() );
  if ( overhead == 0 ) {
    state = QDoubleValidator::validate( str, pos );
  } else {
    if ( str.length() >= overhead && 
         str.startsWith(pref) &&
         endsWith(str, suff) ) {
      QString core = str.mid( pref.length(), str.length() - overhead );
      int corePos = pos - pref.length();
      state = QDoubleValidator::validate( core, corePos );
      pos = corePos + pref.length();
      str.replace( pref.length(), str.length() - overhead, core );
    } else {
      state = QDoubleValidator::validate( str, pos );
      if ( state == Invalid ) {
        // stripWhiteSpace(), cf. QAD_SpinBoxDbl::interpretText()
        QString special = spinBox->specialValueText().stripWhiteSpace();
        QString candidate = str.stripWhiteSpace();

        if ( special.startsWith(candidate) ) {
          if ( candidate.length() == special.length() ) {
            state = Acceptable;
          } else {
            state = Intermediate;
          }
        }
      }
    }
  }
  return state;
}

/*!
  Creates a spin box with min value 0.00, max value 99.99,
  line step 0.1, precision 6, double precision 10e-6, 
  convertion flag 'g' and initial value 0.00.
*/
QAD_SpinBoxDbl::QAD_SpinBoxDbl( QWidget* parent, const char* name )
: QFrame(parent, name),
  QDblRangeControl() 
{
  initSpinBox();
}

/*!
  Constructor
  Creates a spin box with min value <minValue>, max value <maxValue>,
  line step <step>, precision <precision>, double precision <dblPrecision>, 
  convertion flag <cFlag> and initial value <minValue>
*/
QAD_SpinBoxDbl::QAD_SpinBoxDbl( QWidget*    parent, 
			        double      minValue, 
                                double      maxValue, 
                                double      step, 
                                int         precision, 
 			        double      dblPrecision,
			        char        cFlag,
                                const char* name )  
: QFrame( parent, name ),
  QDblRangeControl( minValue, maxValue, step, step, minValue, precision, dblPrecision, cFlag ) 
{
  initSpinBox();
}

/*!
  Destructor
*/
QAD_SpinBoxDbl::~QAD_SpinBoxDbl() 
{}

/*!
  Internal initialization.
*/
void QAD_SpinBoxDbl::initSpinBox()
{
  wrap   = FALSE;
  edited = FALSE;
  butSymbols = UpDownArrows;
  selreq = FALSE;

  up = new QPushButton( this, "up" );
  up->setFocusPolicy( QWidget::NoFocus );
  up->setAutoDefault( FALSE );
  up->setAutoRepeat( TRUE );
  
  down = new QPushButton( this, "down" );
  down->setFocusPolicy( QWidget::NoFocus );
  down->setAutoDefault( FALSE );
  down->setAutoRepeat( TRUE );
  
  validate = new QAD_SpinBoxDblValidator( this, "validator" ); 
  vi = new QLineEdit( this, "line editor" );
  vi->setFrame( FALSE );
  setFocusProxy( vi );
  setFocusPolicy( StrongFocus );
  vi->setValidator( validate );
  vi->installEventFilter( this );
  
  setFrameStyle( Panel | Sunken );
  setLineWidth( 2 );
  
//  setPalettePropagation( AllChildren );
//  setFontPropagation( AllChildren );
  
  setSizePolicy( QSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::Fixed ) );
  updateDisplay();
  
  connect( up,   SIGNAL(pressed()), SLOT(stepUp()) );  
  connect( down, SIGNAL(pressed()), SLOT(stepDown()) );
  connect( vi,   SIGNAL(textChanged(const QString&)), SLOT(textChanged()) );
}

/*!
  Returns the current text of the spin box, including any prefix() and suffix().
*/
QString QAD_SpinBoxDbl::text() const
{
  return vi->text();
}

/*!
  Returns a copy of the current text of the spin box with any prefix
  and/or suffix and white space at the start and end removed.
*/
QString QAD_SpinBoxDbl::cleanText() const
{
  QString s = QString(text()).stripWhiteSpace();
  if ( !prefix().isEmpty() ) {
    QString px = QString(prefix()).stripWhiteSpace();
    int len = px.length();
    if ( len && s.left(len) == px )  // Remove _only_ if it is the prefix
      s.remove( 0, len );
  }
  if ( !suffix().isEmpty() ) {
    QString sx = QString(suffix()).stripWhiteSpace();
    int len = sx.length();
    if ( len && s.right(len) == sx )  // Remove _only_ if it is the suffix
      s.truncate( s.length() - len );
  }
  return s.stripWhiteSpace();
}

/*!
  Sets the special-value text to text.  If set, the spin box will
  display this text instead of a numeric value whenever the current
  value is equal to minVal().  Typically used for indicating that this
  choice has a special (default) meaning.
*/
void QAD_SpinBoxDbl::setSpecialValueText( const QString &text )
{
  specText = text;
  updateDisplay();
}

/*!
  Returns the currently special-value text, or a null string if no
  special-value text is currently set.
*/
QString QAD_SpinBoxDbl::specialValueText() const
{
  if ( specText.isEmpty() )
    return QString::null;
  else
    return specText;
}

/*!
  Sets the prefix to <text>.  The prefix is prepended to the start of
  the displayed value.  Typical use is to indicate the unit of
  measurement to the user.
*/
void QAD_SpinBoxDbl::setPrefix( const QString &text )
{
  pfix = text;
  updateDisplay();
}

/*!
  Sets the suffix to <text>.  The suffix is appended to the end of the
  displayed value.  Typical use is to indicate the unit of measurement
  to the user.
*/
void QAD_SpinBoxDbl::setSuffix( const QString &text )
{
  sfix = text;
  updateDisplay();
}

/*!
  Returns the currently set prefix, or a null string if no prefix is set.
*/
QString QAD_SpinBoxDbl::prefix() const
{
  if ( pfix.isEmpty() )
    return QString::null;
  else
    return pfix;
}

/*!
  Returns the currently set suffix, or a null string if no suffix is set.
*/
QString QAD_SpinBoxDbl::suffix() const
{
  if ( sfix.isEmpty() )
    return QString::null;
  else
    return sfix;
}

/*!
  Setting wrapping to TRUE will allow the value to be stepped from the
  highest value to the lowest, and vice versa.  By default, wrapping is
  turned off.
*/
void QAD_SpinBoxDbl::setWrapping( bool on )
{
  wrap = on;
  updateDisplay();
}

/*!
  Returns the current setWrapping() value.
*/
bool QAD_SpinBoxDbl::wrapping() const
{
  return wrap;
}

/*!
  Reimplementation
*/
QSize QAD_SpinBoxDbl::sizeHint() const
{
  constPolish();
  QFontMetrics fm = fontMetrics();
  int h = vi->sizeHint().height();
  if ( h < 12 )              // ensure enough space for the button pixmaps
    h = 12;
  int w = 35;                // minimum width for the value
  int wx = fm.width( ' ' )*2;
  QString s;

  QAD_SpinBoxDbl* that = (QAD_SpinBoxDbl*)this;
  s = prefix() + that->mapValueToText( that->minValue() ) + suffix();
  w = QMAX( w, fm.width( s ) + wx);
  s = prefix() + that->mapValueToText( that->maxValue() ) + suffix();
  w = QMAX(w, fm.width( s ) + wx );
  if ( !specialValueText().isEmpty() ) {
    s = specialValueText();
    w = QMAX( w, fm.width( s ) + wx );
  }
  QSize r( h * 8/5              // ~ buttons width 
           + w                  // widest value
           + frameWidth() * 2,  // left/right frame
           frameWidth() * 2     // top/bottom frame
           + h                  // font height
         );
  return r.expandedTo( QApplication::globalStrut() );
}

/*!
  Does the layout of the lineedit and the buttons
*/
void QAD_SpinBoxDbl::arrangeWidgets()
{
  if ( !up || !down ) // may happen if the application has a pointer error
    return;
  
  QSize bs; // no, it's short for 'button size'
  bs.setHeight( height()/2 - frameWidth() );
  if ( bs.height() < 8 )
    bs.setHeight( 8 );
  bs.setWidth( bs.height() * 8 / 5 ); // 1.6 - approximate golden mean
  setFrameRect( QRect( 0, 0, 0, 0 ) );
  
  if ( up->size() != bs || down->size() != bs ) {
    up->resize( bs );
    down->resize( bs );
    updateButtonSymbols();
  }
  
  int y = frameWidth();
  int x = width() - y - bs.width();
  up->move( x, y );
  down->move( x, height() - y - up->height() );
  vi->setGeometry( frameWidth(), frameWidth(),
		   x - frameWidth(), height() - 2*frameWidth() );
}

/*!
  Sets the current value of the spin box to <value>.  This is
  QRangeControl::setValue() made available as a slot.
*/
void QAD_SpinBoxDbl::setValue( double value )
{
  QDblRangeControl::setValue( value );
}

/*!
  Increases the current value one step, wrapping as necessary.  This is
  the same as clicking on the pointing-up button, and can be used for
  e.g.  keyboard accelerators.
*/
void QAD_SpinBoxDbl::stepUp()
{
  if ( edited )
    interpretText();
  if ( wrapping() && ( value()+lineStep() > maxValue() ) )
    setValue( minValue() );
  else
    addLine();
}

/*!
  Decreases the current value one step, wrapping as necessary.  This is
  the same as clicking on the pointing-down button, and can be used
  for e.g.  keyboard accelerators.
*/
void QAD_SpinBoxDbl::stepDown()
{
  if ( edited )
    interpretText();
  if ( wrapping() && ( value()-lineStep() < minValue() ) )
    setValue( maxValue() );
  else
    subtractLine();
}

/*!
  Intercepts and handles those events coming to the embedded QLineEdit
  which have special meaning for the QAD_SpinBoxDbl.
*/
bool QAD_SpinBoxDbl::eventFilter( QObject* obj, QEvent* ev )
{
  if ( obj != vi )
    return FALSE;
  
  if ( ev->type() == QEvent::KeyPress ) {
    QKeyEvent* k = (QKeyEvent*)ev;
    
    bool retval = FALSE; // workaround for MSVC++ optimization bug
    if( (k->key() == Key_Tab) || (k->key() == Key_BackTab) ){
      if ( edited )
        interpretText();
      qApp->sendEvent( this, ev );
      retval = TRUE;
    } if ( k->key() == Key_Up ) {
      stepUp();
      retval = TRUE;
    } else if ( k->key() == Key_Down ) {
      stepDown();
      retval = TRUE;
    } else if ( k->key() == Key_Return ) {
      interpretText();
      return FALSE;
    }
    if ( retval )
      return retval;
  } else if ( ev->type() == QEvent::FocusOut || ev->type() == QEvent::Leave || ev->type() == QEvent::Hide ) {
    if ( edited ) {
      interpretText();
    }
    return FALSE;
  }
  return FALSE;
}

/*!
  Reimplementation
*/
void QAD_SpinBoxDbl::leaveEvent( QEvent* )
{
  if ( edited )
    interpretText();
}

/*!
  Reimplementation
*/
void QAD_SpinBoxDbl::resizeEvent( QResizeEvent* )
{
  arrangeWidgets();
}

/*!
  Reimplementation
*/
void QAD_SpinBoxDbl::wheelEvent( QWheelEvent * e )
{
  e->accept();
  static float offset = 0;
  static QAD_SpinBoxDbl* offset_owner = 0;
  if (offset_owner != this) {
    offset_owner = this;
    offset = 0;
  }
  offset += -e->delta()/120;
  if (QABS(offset) < 1)
    return;
  int ioff = int(offset);
  int i;
  for (i=0; i<QABS(ioff); i++)
    offset > 0 ? stepDown() : stepUp();
  offset -= ioff;
}

/*!
  This method gets called by QRangeControl whenever the value has changed.
  Updates the display and emits the valueChanged() signals.
*/
void QAD_SpinBoxDbl::valueChange()
{
  selreq = hasFocus();
  updateDisplay();
  selreq = FALSE;
  emit valueChanged( value() );
  emit valueChanged( currentValueText() );
}

/*!
  This method gets called by QRangeControl whenever the range has
  changed.  It adjusts the default validator and updates the display.
*/
void QAD_SpinBoxDbl::rangeChange()
{
  updateDisplay();
}

/*!
  Sets the validator to <v>.  The validator controls what keyboard
  input is accepted when the user is editing in the value field.  The
  default is to use a suitable QIntValidator.
*/
void QAD_SpinBoxDbl::setValidator( const QValidator* v )
{
  if ( vi )
    vi->setValidator( v );
}
/*!
  Returns the validator which constrains editing for this spin box if
  there is any, or else 0.
*/
const QValidator* QAD_SpinBoxDbl::validator() const
{
  return vi ? vi->validator() : 0;
}

/*!
  Updates the contents of the embedded QLineEdit to reflect current
  value, using mapValueToText().  Also enables/disables the push
  buttons accordingly.
*/
void QAD_SpinBoxDbl::updateDisplay()
{
  vi->setUpdatesEnabled( FALSE );
  vi->setText( currentValueText() );
  if ( selreq && isVisible() && ( hasFocus() || vi->hasFocus() ) ) {
    selectAll();
  } else {
    if ( !suffix().isEmpty() && 
	 endsWith(vi->text(), suffix()) )
      vi->setCursorPosition( vi->text().length() - suffix().length() );
  }
  vi->setUpdatesEnabled( TRUE );
  vi->repaint( FALSE ); // immediate repaint needed for some reason
  edited = FALSE;
 
  up->setEnabled( isEnabled() && (wrapping() || value() < maxValue()) );
  down->setEnabled( isEnabled() && (wrapping() || value() > minValue()) );
  vi->setEnabled( isEnabled() );
}

/*!
  QAD_SpinBoxDbl calls this after the user has manually edited the contents
  of the spin box (not using the up/down buttons/keys).
  The default implementation of this function interprets the new text
  using mapTextToValue().  If mapTextToValue() is successful, it
  changes the spin box' value.  If not the value if left unchanged.
*/
void QAD_SpinBoxDbl::interpretText()
{
  bool ok = TRUE;
  bool done = FALSE;
  double newVal = 0;
  if ( !specialValueText().isEmpty() ) {
    QString s = QString(text()).stripWhiteSpace();
    QString t = QString(specialValueText()).stripWhiteSpace();
    if ( s == t ) {
      newVal = minValue();
      done = TRUE;
    }
  }
  if ( !done )
    newVal = mapTextToValue( &ok );
  if ( ok )
    setValue( newVal );
  updateDisplay();    // Sometimes redundant
}

/*!
  Returns a pointer to the embedded 'up' button.
*/

QPushButton* QAD_SpinBoxDbl::upButton() const
{
  return up;
}

/*!
  Returns a pointer to the embedded 'down' button.
*/
QPushButton* QAD_SpinBoxDbl::downButton() const
{
  return down;
}

/*!
  Returns a pointer to the embedded QLineEdit.
*/
QLineEdit* QAD_SpinBoxDbl::editor() const
{
  return vi;
}

/*!
  This slot gets called whenever the user edits the text of the spin box.
*/
void QAD_SpinBoxDbl::textChanged()
{
  edited = TRUE;  // This flag is cleared in updateDisplay()
};

/*!
  This virtual function is used by the spin box whenever it needs to
  display value <v>.  The default implementation returns a string
  containing <v> printed in the standard way.
*/

QString QAD_SpinBoxDbl::mapValueToText( double v )
{
  QString s;
  s.setNum( v, convertFlag(), precision() );
  return s;
}

/*!
  This virtual function is used by the spin box whenever it needs to
  interpret the text entered by the user as a value.  The default
  implementation tries to interpret it as an integer in the standard
  way, and returns the double value.
*/
double QAD_SpinBoxDbl::mapTextToValue( bool* ok )
{
  QString s = text();
  double newVal = s.toDouble( ok );
  if ( !(*ok) && !( !prefix() && !suffix() ) ) {// Try removing any pre/suffix
    s = cleanText();
    newVal = s.toDouble( ok );
  }
  return newVal;
}

/*!
  Returns the full text calculated from the current value, including any
  prefix, suffix or special-value text.
*/
QString QAD_SpinBoxDbl::currentValueText()
{
  QString s;
  if ( (value() <= minValue()) && !specialValueText().isEmpty() ) {
    s = specialValueText();
  } else {
    s = prefix();
    s.append( mapValueToText( value() ) );
    s.append( suffix() );
  }
  return s;
}

/*!
  Reimplementation
*/
void QAD_SpinBoxDbl::setEnabled( bool on )
{
  bool b = isEnabled();
  QFrame::setEnabled( on );
  if ( isEnabled() != b ) {
    // ## enabledChange() might have been a better choice
    updateDisplay();
  }
}

/*!
  Reimplementation
*/
void QAD_SpinBoxDbl::styleChange( QStyle& old )
{
  setFrameStyle( Panel | Sunken );
  arrangeWidgets();
  QWidget::styleChange( old );
}

/*!  
  Sets the spin box to display <newSymbols> on its buttons. 
  <newSymbols> can be either <UpDownArrows> (the default) or <PlusMinus>.
*/
void QAD_SpinBoxDbl::setButtonSymbols( ButtonSymbols newSymbols )
{
  if ( buttonSymbols() == newSymbols )
    return;
  butSymbols = newSymbols;
  updateButtonSymbols();
}

/*!  
  Returns the current button symbol mode.  The default is <UpDownArrows>
*/
QAD_SpinBoxDbl::ButtonSymbols QAD_SpinBoxDbl::buttonSymbols() const
{
  return butSymbols;
}

/*!
  This function uses the pixmap cache for a Different Reason: the
  pixmap cache also preserves QPixmap::serialNumber().  by doing
  this, QButton::setPixmap() is able to avoid flicker e.g. when the
  spin box is resized in such a way that the height of the buttons
  does not change (common the default size policy).
*/
void QAD_SpinBoxDbl::updateButtonSymbols()
{
  QString key( QString::fromLatin1( "$qt$QAD_SpinBoxDbl$" ) );
  bool pmSym = buttonSymbols() == PlusMinus;
  key += QString::fromLatin1( pmSym ? "+-" : "^v" );
  key += QString::number( down->height() );
  QString upKey = key + QString::fromLatin1( "$up" );
  QString dnKey = key + QString::fromLatin1( "$down" );
  QBitmap upBm;
  QBitmap dnBm;
  
  bool found = QPixmapCache::find( dnKey, dnBm )
    && QPixmapCache::find( upKey, upBm );
  
  if ( !found ) {
    QPainter p;
    if ( pmSym ) {
      int h = down->height()-4;
      if ( h < 3 )
        return;
      else if ( h == 4 )
        h = 3;
      else if ( (h > 6) && (h & 1) )
        h--;
      h -= ( h / 8 ) * 2;    // Empty border
      dnBm.resize( h, h );
      p.begin( &dnBm );
      p.eraseRect( 0, 0, h, h );
      p.setBrush( color1 );
      int c = h/2;
      p.drawLine( 0, c, h, c );
      if ( !(h & 1) )
        p.drawLine( 0, c-1, h, c-1 );
      p.end();
      upBm = dnBm;
      p.begin( &upBm );
      p.drawLine( c, 0, c, h );
      if ( !(h & 1) )
        p.drawLine( c-1, 0, c-1, h );
      p.end();
    }
    else {
      int w = down->width()-4;
      if ( w < 3 )
        return;
      else if ( !(w & 1) )
        w--;
      w -= ( w / 7 ) * 2;    // Empty border
      int h = w/2 + 2;        // Must have empty row at foot of arrow
      dnBm.resize( w, h );
      p.begin( &dnBm );
      p.eraseRect( 0, 0, w, h );
      QPointArray a;
      a.setPoints( 3,  0, 1,  w-1, 1,  h-2, h-1 );
      p.setBrush( color1 );
      p.drawPolygon( a );
      p.end();
#ifndef QT_NO_TRANSFORMATIONS
      QWMatrix wm;
      wm.scale( 1, -1 );
      upBm = dnBm.xForm( wm );
#else
      upBm.resize( w, h );
      p.begin( &upBm );
      p.eraseRect( 0, 0, w, h );
      a.setPoints( 3,  0, h-2,  w-1, h-2,  h-2, 0 );
      p.setBrush( color1 );
      p.drawPolygon( a );
      p.end();
#endif
    }
    QPixmapCache::insert( dnKey, dnBm );
    QPixmapCache::insert( upKey, upBm );
  }
  down->setPixmap( dnBm );
  up->setPixmap( upBm );
}

/*!
  Returns minimum value, reimplementaion
*/
double QAD_SpinBoxDbl::minValue() 
{
  return QDblRangeControl::minValue();  
}

/*!
  Returns maximum value, reimplementaion
*/
double QAD_SpinBoxDbl::maxValue() 
{
  return QDblRangeControl::maxValue();  
}

/*!
  Sets minimum value, reimplementaion
*/
void QAD_SpinBoxDbl::setMinValue( double minValue ) 
{
  QDblRangeControl::setMinValue( minValue );
}

/*!
  Sets maximum value, reimplementaion
*/
void QAD_SpinBoxDbl::setMaxValue( double maxValue ) 
{
  QDblRangeControl::setMaxValue( maxValue );
}

/*!
  Returns step size, reimplementaion
*/
double QAD_SpinBoxDbl::lineStep() 
{
  return QDblRangeControl::lineStep();  
}

/*!
  Sets step size
*/
void QAD_SpinBoxDbl::setLineStep( double step ) 
{
  setSteps( step, pageStep() );
}

/*!
  Returns value of the spin box, reimplementaion
*/
double QAD_SpinBoxDbl::value()
{
  QAD_SpinBoxDbl* that = ( QAD_SpinBoxDbl* ) this;
  if ( edited ) {
    that->edited = FALSE;  // avoid recursion
    that->interpretText();
  }
  return QDblRangeControl::value();
}

/*! 
  Selects all the text in the editor of the spinbox. 
*/
void QAD_SpinBoxDbl::selectAll()
{
  int overhead = prefix().length() + suffix().length();
  if ( !overhead || currentValueText() == specialValueText() ) {
    vi->selectAll();
  } else {
    vi->setSelection( prefix().length(), vi->text().length() - overhead );
  }
}
