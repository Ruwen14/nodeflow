import random

from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *

from PyQt5.QtWebEngineWidgets import *

import sys
import math
from typing import List, Tuple


def create_variable_icon(color):
    # Create a rounded version of the original pixmap

    pixmap = QPixmap(100, 50)

    mask = QBitmap((pixmap.size()))
    mask.fill(Qt.color0)

    painter = QPainter(mask)
    painter.setRenderHint(QPainter.Antialiasing)
    painter.setBrush(Qt.color1)
    painter.drawRoundedRect(mask.rect(), 30, 30)
    painter.end()

    pixmap.fill(color)
    pixmap.setMask(mask)

    return QIcon(pixmap)


class CollapsibleBox(QWidget):
    def __init__(self, title="", parent=None):
        super(CollapsibleBox, self).__init__(parent)

        self.toggle_button = QToolButton(
            text=title, checkable=True, checked=False
        )

        font = QApplication.font()
        font.setPointSize(8)
        font.setBold(True)
        self.toggle_button.setFont(font)
        self.toggle_button.setStyleSheet("""
                                    QToolButton 
                                    {
                                        border: none; 
                                        color: lightGray; 
                                        background-color: rgb(56, 56, 56);   
                                        border-radius: 0px;
                                        padding: 5px;
                                    }
                                    
                                    
                                    """)

        self.toggle_button.setToolButtonStyle(
            Qt.ToolButtonTextBesideIcon
        )

        self.toggle_button.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Fixed)

        self.toggle_button.setArrowType(Qt.RightArrow)
        self.toggle_button.pressed.connect(self.on_pressed)

        self.toggle_animation = QParallelAnimationGroup(self)

        self.content_area = QScrollArea(
            maximumHeight=0, minimumHeight=0
        )
        self.content_area.setSizePolicy(
            QSizePolicy.Expanding, QSizePolicy.Fixed
        )
        self.content_area.setFrameShape(QFrame.NoFrame)

        lay = QVBoxLayout(self)
        lay.setSpacing(0)
        lay.setContentsMargins(0, 0, 0, 0)
        lay.addWidget(self.toggle_button)
        lay.addWidget(self.content_area)

        self.toggle_animation.addAnimation(
            QPropertyAnimation(self, b"minimumHeight")
        )
        self.toggle_animation.addAnimation(
            QPropertyAnimation(self, b"maximumHeight")
        )
        self.toggle_animation.addAnimation(
            QPropertyAnimation(self.content_area, b"maximumHeight")
        )

    @pyqtSlot()
    def on_pressed(self):
        checked = self.toggle_button.isChecked()
        self.toggle_button.setArrowType(
            Qt.DownArrow if not checked else Qt.RightArrow
        )
        self.toggle_animation.setDirection(
            QAbstractAnimation.Forward
            if not checked
            else QAbstractAnimation.Backward
        )
        self.toggle_animation.start()

    def setContentLayout(self, layout):
        lay = self.content_area.layout()
        del lay
        self.content_area.setLayout(layout)
        collapsed_height = (
                self.sizeHint().height() - self.content_area.maximumHeight()
        )
        content_height = layout.sizeHint().height()
        for i in range(self.toggle_animation.animationCount()):
            animation = self.toggle_animation.animationAt(i)
            animation.setDuration(0)
            animation.setStartValue(collapsed_height)
            animation.setEndValue(collapsed_height + content_height)

        content_animation = self.toggle_animation.animationAt(
            self.toggle_animation.animationCount() - 1
        )
        content_animation.setDuration(0)
        content_animation.setStartValue(0)
        content_animation.setEndValue(content_height)


class RectItem(QGraphicsRectItem):

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self.setBrush(QGradient(QGradient.WinterNeva))
        self.itemMoved = pyqtSignal(QPointF)

        self.setFlag(QGraphicsRectItem.ItemIsSelectable)
        self.setFlag(QGraphicsRectItem.ItemIsMovable)
        self.setFlag(QGraphicsRectItem.ItemSendsScenePositionChanges)
        self.setFlag(QGraphicsRectItem.ItemIsFocusable)
        # self.setFlag(QGraphicsRectItem.ItemDoesntPropagateOpacityToChildren)
        self.setAcceptDrops(True)
        self.setAcceptHoverEvents(True)

        self.lineEdit = QLineEdit()
        self.lineEdit.setAlignment(Qt.AlignCenter)

        self.proxyWidget = QGraphicsProxyWidget(self)
        self.proxyWidget.setWidget(self.lineEdit)
        # self.proxyWidget.setMaximumWidth(self.boundingRect().width()*2)

        self.proxyWidget.setPos((self.boundingRect().center() - self.proxyWidget.boundingRect().center() + QPointF(0,
                                                                                                                   -self.boundingRect().height() / 1.2)))

        #

    def mousePressEvent(self, event: QGraphicsSceneMouseEvent) -> None:
        if event.button() == Qt.RightButton:
            print("Right")

        super().mousePressEvent(event)

    def itemChange(self, change: QGraphicsItem.GraphicsItemChange, value: QVariant) -> QVariant:

        if (
                change == QGraphicsItem.ItemPositionChange and self.scene()) or change == QGraphicsItem.ItemScenePositionHasChanged:
            # self.itemMoved.emit
            pass

        return super().itemChange(change, value)


class VectorInputWidget(QWidget):
    def __init__(self):
        super().__init__()

        self.spinboxes = []
        self.vector = [0, 0, 0]

        self.init_ui()
        self.layout().setContentsMargins(0, 0, 0, 0)

    def init_ui(self):
        layout = QHBoxLayout(self)
        for i in range(3):
            spinbox = QLineEdit()
            spinbox.setPlaceholderText(["X", "Y", "Z"][i])
            # spinbox.setMinimum(-1000)
            # spinbox.setMaximum(1000)
            self.spinboxes.append(spinbox)
            layout.addWidget(spinbox)


class QtGraphicsNodeItem(QGraphicsObject):
    def __init__(self, intputPins: int, outputPins: int, hasExecPins: bool, name: str, category: str, width=450,
                 height=450, **kwargs):
        super().__init__(**kwargs)

        self.setFlag(QGraphicsItem.ItemIsSelectable)
        self.setFlag(QGraphicsItem.ItemIsMovable)
        self.setFlag(QGraphicsItem.ItemSendsScenePositionChanges)
        self.setFlag(QGraphicsItem.ItemIsFocusable)
        self.setAcceptDrops(True)
        self.setAcceptHoverEvents(True)

        self.m_Width = width
        self.m_Height = height
        self.m_Roundness = 8

        self.inputPins = intputPins
        self.outputPins = outputPins
        self.hasExecPin = hasExecPins
        self.name = name
        self.category = category

        self.setupPins()

    def setupPins(self):
        pinRadius = 10
        pinDiameter = 2 * pinRadius
        pinOffset = 2.4 * pinDiameter

        pinCoord: QPointF = QPointF(pinDiameter,
                                    self.m_Height * 0.15 + pinDiameter)

        if self.hasExecPin:
            inputExecPin = QtGraphicsExecPinItem(pinCoord, self)
            outputExecPin = QtGraphicsExecPinItem(
                QPointF(self.m_Width - pinOffset, pinCoord.y()),
                self
            )

            inputExecPin.setConnected(False)
            outputExecPin.setConnected(False)

            pinCoord = QPointF(inputExecPin.sceneBoundingRect().center().x() - 0.04 * pinOffset,
                               inputExecPin.sceneBoundingRect().center().y() + pinOffset)

        inPinCoord: QPointF = pinCoord
        for inPinIdx in range(1, self.inputPins + 1):
            inPin = QtGraphicsPinItem(PinType.Input, self)
            inPin.setPos(inPinCoord)
            inPinCoord = QPointF(inPinCoord.x(), inPinCoord.y() + pinOffset)

        outPinCord: QPointF = QPointF(self.m_Width - pinOffset + 0.1 * pinOffset, pinCoord.y())
        for outPinIdx in range(1, self.outputPins + 1):
            outPin = QtGraphicsPinItem(PinType.Output, self)
            outPin.setPos(outPinCord)
            outPinCord = QPointF(outPinCord.x(), outPinCord.y() + pinOffset)

    def changeText(self, text):
        print(text)
        print(self.validator.validate(text, 0))

    def setInputControllForPin(self, pinIndex, input_type: str, color: str = None):
        inputPins = [item for item in self.childItems() if
                     isinstance(item, QtGraphicsPinItem) and item.pinType == PinType.Input]

        # "background-color: rgb(35, 38, 41); border: 1px solid rgb(71, 74, 78); color:lightGray; border-radius: 5px;")
        # rgb(203, 203, 203)
        pin = inputPins[pinIndex]
        if input_type == "StringInput":
            self.inputLineControl = QLineEdit()
            self.validator = QIntValidator(-2147483648, 2147483647, self)
            self.inputLineControl.setValidator(self.validator)
            self.inputLineControl.setAlignment(Qt.AlignLeft)
            self.inputLineControl.textChanged.connect(self.changeText)
            # self.inputLineControl.setText("0.1")

            if color == "gray":
                self.inputLineControl.setStyleSheet(
                    "background-color: rgb(203, 203, 203); border: 0px solid black; color: rgb(44, 44, 44); border-radius: 5px;")
            else:
                self.inputLineControl.setStyleSheet(
                    "background-color: rgb(35, 38, 41); border: 1px solid rgb(71, 74, 78); color:lightGray; border-radius: 5px")

            self.inputLineControl.setAttribute(Qt.WA_TranslucentBackground)
            self.inputLineControl.setFixedWidth(100)

            font = QApplication.font()
            font.setPixelSize(10 * 1.6)
            font.setStyleStrategy(QFont.PreferAntialias)
            font.setHintingPreference(QFont.HintingPreference.PreferNoHinting)
            font.setBold(False)
            self.inputLineControl.setFont(font)
            self.inputLineControlProxy = QGraphicsProxyWidget(self)
            self.inputLineControlProxy.setWidget(self.inputLineControl)

            self.inputLineControlProxy.setPos(
                pin.pos() + QPointF(QFontMetricsF(pin.m_font).width(pin._displayName) + pin.radius * 3.8,
                                    pin.boundingRect().topLeft().y()))
            self.inputLineControl.setFixedHeight(pin.radius * 2.8)

        elif input_type == "BoolInput":
            self.inputBoolControl = QCheckBox()
            self.inputBoolControl.setChecked(False)
            self.inputBoolControl.setStyleSheet(f'''
                                                                            QCheckBox
                                                                            {{
                                                                                background-color: rgb(35, 38, 41);
                                                                                color: rgb(14, 134, 255); /* lightGray */
                                                                                border: 1px solid rgb(71, 74, 78);
                                                                                border-radius: 5px;                                                                           
                                                                                padding: 1px;
                                                                                spacing: 0px;
                                                                                                                                                        
                                                                            }}

                                                                            QCheckBox::indicator
                                                                            {{
                                                                                width: {pin.radius * 2.6} px;
                                                                                height: {pin.radius * 2.6} px;
                                                                            }}
                                                                            
                                                                         
                                                                            
                                                                            ''')
            self.inputBoolControl.setAttribute(Qt.WA_TranslucentBackground)

            # self.inputBoolControl.setStyleSheet("background-color: rgb(51, 51, 51); border: 1px solid transparent; QCheckBox::indicator { border: 1px solid; border-color: yellow; }")
            self.inputBoolControlProxy = QGraphicsProxyWidget(self)
            self.inputBoolControlProxy.setWidget(self.inputBoolControl)

            self.inputBoolControlProxy.setPos(
                pin.pos() + QPointF(QFontMetricsF(pin.m_font).width(pin._displayName) + pin.radius * 3.8,
                                    pin.boundingRect().topLeft().y()))


        elif input_type == "IntInput":
            self.inputIntControl = QDoubleSpinBox()
            self.inputIntControl.setStyleSheet("background-color: rgb(35, 38, 41); color:lightGray;")

            self.inputIntControl.setAttribute(Qt.WA_TranslucentBackground)
            self.inputIntControl.setFixedWidth(100)

            font = QApplication.font()
            font.setPixelSize(10 * 1.8)
            font.setStyleStrategy(QFont.PreferAntialias)
            font.setHintingPreference(QFont.HintingPreference.PreferNoHinting)
            font.setBold(True)
            self.inputIntControl.setFont(font)
            self.inputIntControlProxy = QGraphicsProxyWidget(self)
            self.inputIntControlProxy.setWidget(self.inputIntControl)

            self.inputIntControlProxy.setPos(
                pin.pos() + QPointF(QFontMetricsF(pin.m_font).width(pin._displayName) + pin.radius * 3.8,
                                    pin.boundingRect().topLeft().y()))
            self.inputIntControl.setFixedHeight(pin.radius * 2.8)


        else:
            self.inputVector3DControl = VectorInputWidget()
            self.inputVector3DControl.setAttribute(Qt.WA_TranslucentBackground)
            self.inputVector3DControl.setFixedWidth(200)
            self.inputVector3DControl.setStyleSheet("background-color: rgb(35, 38, 41); color:lightGray;")

            font = QApplication.font()
            font.setPixelSize(10 * 1.8)
            font.setStyleStrategy(QFont.PreferAntialias)
            font.setHintingPreference(QFont.HintingPreference.PreferNoHinting)
            font.setBold(True)
            self.inputVector3DControl.setFont(font)
            self.inputVector3DControlProxy = QGraphicsProxyWidget(self)
            self.inputVector3DControlProxy.setWidget(self.inputVector3DControl)

            self.inputVector3DControlProxy.setPos(
                pin.pos() + QPointF(QFontMetricsF(pin.m_font).width(pin._displayName) + pin.radius * 3.8,
                                    pin.boundingRect().topLeft().y()))

    def boundingRect(self):
        paddingFactor = 0.05
        return QRectF(0 - self.m_Width * paddingFactor,
                      0 - self.m_Height * paddingFactor,
                      self.m_Width + 2 * paddingFactor * self.m_Width,
                      self.m_Height + 2 * paddingFactor * self.m_Height)

    def getLabelBoundingRect(self):
        return QRectF(0, 0, self.m_Width, self.m_Height * 0.15)

    def paintLabel(self, painter: QPainter, style: QStyleOptionGraphicsItem, widget=None):
        startColor = QColor(134, 29, 26, 210)

        k = 24, 140, 126
        drawShape = QRectF(0, 0, self.m_Width, self.m_Height * 0.15)

        grad = QLinearGradient(QPointF(0, 0), QPointF(0 + self.m_Width, 0 + self.m_Height))
        grad.setColorAt(0, startColor)
        grad.setColorAt(0.5, startColor)
        grad.setColorAt(1, Qt.transparent)
        painter.setPen(QPen(Qt.black, 2))

        painter.setBrush(grad)
        painter.drawRect(drawShape)

        # draw text label
        painter.setPen(Qt.lightGray)
        f: QFont = painter.font()
        f.setHintingPreference(QFont.HintingPreference.PreferNoHinting)

        f.setPointSize(18)
        f.setBold(True)
        f.setItalic(False)
        painter.setFont(f)
        painter.drawText(QPointF(drawShape.left() + drawShape.width() * 0.05, drawShape.center().y()), self.name)
        if self.category is not None:
            f.setPointSize(14)
            f.setItalic(True)
            f.setBold(False)
            painter.setFont(f)
            painter.drawText(QPointF(drawShape.left() + drawShape.width() * 0.05, drawShape.center().y() + 20),
                             self.category)

    def paintBody(self, painter: QPainter, style: QStyleOptionGraphicsItem, widget=None):
        drawShape = QRectF(0, self.m_Height * 0.15, self.m_Width, self.m_Height - self.m_Height * 0.15)
        backgroundColor = QColor(51, 51, 51)

        painter.setPen(QPen(Qt.black, 2))
        painter.setBrush(backgroundColor)
        painter.drawRect(drawShape)

    def paint(self, painter: QPainter, style: QStyleOptionGraphicsItem, widget=None):
        painter.setClipRect(style.exposedRect)

        self.paintLabel(painter, style, widget)
        self.paintBody(painter, style, widget)

        painter.setPen(QPen(QColor(225, 163, 0), 1))
        painter.setBrush(QColor(Qt.transparent))

        # painter.drawRect(self.boundingRect())

        # return
        if self.isSelected():
            painter.setPen(QPen(QColor(225, 163, 0), 3))
            painter.setBrush(QColor(Qt.transparent))
            painter.drawRect(QRectF(0, 0, self.m_Width, self.m_Height))

    def contextMenuEvent(self, event: 'QGraphicsSceneContextMenuEvent') -> None:
        self.setSelected(True)

        menu = QMenu()
        menu.addAction("Node Action 1")
        menu.addAction("Node Action 2")
        menu.exec(event.screenPos())
        self.setSelected(False)


class ConstantNode(QtGraphicsNodeItem):
    def __init__(self, **kwargs):
        super().__init__(intputPins=0, outputPins=2, hasExecPins=False, name="Constant", category=None, width=300,
                         height=250)

        self.lineEdit = QLineEdit()
        self.lineEdit.setAlignment(Qt.AlignRight)

        self.lineEdit.setStyleSheet("background-color: rgb(77, 78, 79); border: 0px solid lightGray; color:lightGray;")
        self.m_font: QFont = QFont()
        self.m_font.setPixelSize(10 * 1.8)
        self.m_font.setStyleStrategy(QFont.PreferAntialias)
        self.m_font.setHintingPreference(QFont.HintingPreference.PreferNoHinting)
        self.lineEdit.setFont(self.m_font)
        self.lineEdit.setFixedWidth(100)

        # validator = QDoubleValidator(0, 100, 2, self)
        # validator.setNotation(QDoubleValidator.StandardNotation)
        #
        # self.lineEdit.setValidator(validator)

        self.proxyLineEdit = QGraphicsProxyWidget(self)
        self.proxyLineEdit.setWidget(self.lineEdit)

        outPins = [item for item in self.childItems() if isinstance(item, QtGraphicsPinItem)]
        self.proxyLineEdit.setPos(outPins[0].pos() + QPointF(-self.lineEdit.width() - outPins[0].radius * 2,
                                                             outPins[0].boundingRect().topLeft().y()))
        self.lineEdit.setFixedHeight(outPins[0].radius * 2.8)

        outPins[0]._displayName = ""
        outPins[1]._displayName = ""

        # --------------------------------------------------------------------------------------------------------------------
        self.boolCheckBox = QCheckBox()
        self.boolCheckBox.setChecked(False)

        self.boolCheckBox.setStyleSheet(f'''
                                        QCheckBox
                                        {{
                                            background-color: rgb(77, 78, 79);
                                            color: lightGray; /* rgb(66, 153, 255) */
                                            border: none;
                                            spacing: 0px;
                                            padding: -1px;
                                        }}
                                        
                                        QCheckBox::indicator
                                        {{
                                            width: {outPins[0].radius * 2.8} px;
                                            height: {outPins[0].radius * 2.8} px;
                                        }}
                                        ''')

        # self.boolCheckBox.setStyleSheet("background-color: rgb(51, 51, 51); border: 1px solid transparent; QCheckBox::indicator { border: 1px solid; border-color: yellow; }")
        self.boolCheckBoxProxy = QGraphicsProxyWidget(self)
        self.boolCheckBoxProxy.setWidget(self.boolCheckBox)

        self.boolCheckBoxProxy.setPos(outPins[1].pos() + QPointF(-self.boolCheckBox.width() - outPins[1].radius * 2,
                                                                 outPins[1].boundingRect().topLeft().y()))


class QtGraphicsExecPinItem(QGraphicsItem):
    def __init__(self, pos: QPointF, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self.setFlag(QGraphicsItem.ItemIsSelectable)
        self.setFlag(QGraphicsItem.ItemSendsScenePositionChanges)
        self.setFlag(QGraphicsItem.ItemIsFocusable)
        self.setAcceptDrops(True)
        self.setAcceptHoverEvents(True)

        self.targetPos: QPointF = QPointF(0, 0)

        self.width = 20
        self.height = 24
        self.m_Connected = False

        self.setPos(pos)

    def setConnected(self, connected: bool):
        self.m_Connected = connected
        self.update()

    def isConnected(self) -> bool:
        return self.m_Connected

    def boundingRect(self) -> QRectF:
        return QRectF(self.targetPos.x() - self.width * 0.2,
                      self.targetPos.y() - self.height * 0.2,
                      self.width + 2 * self.width * 0.2,
                      self.height + 2 * self.height * 0.2)

    def paint(self, painter: QPainter, option: QStyleOptionGraphicsItem, widget=None) -> None:
        # painter.drawRect(self.boundingRect())
        drawPath = QPainterPath(self.targetPos)
        drawPath.lineTo(self.width / 2, drawPath.currentPosition().y())
        drawPath.lineTo(self.width, self.height / 2)
        drawPath.lineTo(self.width / 2, self.height)
        drawPath.lineTo(self.targetPos.x(), self.height)
        drawPath.closeSubpath()

        if self.isSelected():
            painter.setPen(QPen(QColor(225, 163, 0), 3))
        else:
            painter.setPen(QPen(Qt.lightGray, 3))

        painter.setBrush(QColor(51, 51, 51))
        if self.m_Connected:
            painter.setBrush(Qt.lightGray)

        painter.drawPath(drawPath)

        # painter.drawRect(self.boundingRect())

    def contextMenuEvent(self, event: 'QGraphicsSceneContextMenuEvent') -> None:
        return
        # super().contextMenuEvent(event)


class PinType:
    Input = 1
    Output = 2


class QtGraphicsPinItem(QGraphicsItem):
    def __init__(self, pinType: int, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self.setFlag(QGraphicsItem.ItemIsSelectable)
        self.setFlag(QGraphicsItem.ItemSendsScenePositionChanges)
        self.setFlag(QGraphicsItem.ItemIsFocusable)
        self.setAcceptDrops(True)
        self.setAcceptHoverEvents(True)

        self.targetPos: QPointF = QPointF(0, 0)

        self.radius = 10
        self.pinType: int = pinType

        self.m_font: QFont = QFont()
        self.m_font.setBold(True)
        self.m_font.setPixelSize(self.radius * 1.8)
        self.m_font.setHintingPreference(QFont.HintingPreference.PreferNoHinting)

        self._portDrawShape: QRectF = self.computePortBoundingRect(self.targetPos)
        self._dirIndicatorDrawShape: QRectF = self.computeTriangleIndicatorBoundingRect(self.targetPos)
        self._displayName = random.choice(["Attack Time", "Decay Time", "Count", "Value", "Index", "Array"])
        self.m_dirIndicatorDrawPath: QPainterPath = self.computeTriangleIndicatorDrawPath(self.targetPos)

        self.m_boundingRect: QRectF = self.computeBoundingRect(self.targetPos, padding=0.2)
        self.m_color = random.choice([QColor(153, 193, 115), QColor(149, 59, 75), QColor(86, 165, 147)])
        # self.m_color = random.choice([QColor(186,163,187), QColor(110,248,114), QColor(128,97,124), QColor(238,200,104), QColor(165,251,248), QColor(102,131,254), QColor(245,101,156), QColor(229,128,253), QColor(250,231,215), QColor(103,213,215), QColor(154,202,146), QColor(102,149,162), QColor(165,148,98), QColor(223,241,150), QColor(159,152,245), QColor(148,114,180), QColor(212,189,232), QColor(145,254,175), QColor(215,169,131), QColor(178,101,224), QColor(111,183,97), QColor(246,142,199), QColor(202,108,106), QColor(161,220,199), QColor(179,223,106), QColor(209,130,165), QColor(145,188,224), QColor(242,184,169), QColor(116,183,188), QColor(127,139,212)])
        # self.m_color = random.choice([QColor('#f9fbe7'), QColor('#e2f2e3'), QColor('#fad4e1'), QColor('#f4f7d1'), QColor('#c5ebfd'), QColor('#fffbd2'), QColor('#ccd0ec'), QColor('#ffc9b8'), QColor('#f4f4f4'), QColor('#ddb5e4'), QColor('#ccbeba'), QColor('#d9ade1'), QColor('#c9bbb6'), QColor('#aeb6e0'), QColor('#cce5b0'), QColor('#c4b4af'), QColor('#b3c1c7'), QColor('#f19fa0'), QColor('#f493b4'), QColor('#ffa88d'), QColor('#a1d4a4'), QColor('#76dbe8'), QColor('#76dbe8'), QColor('#b7a39d'), QColor('#eb8a8a'), QColor('#ff916e'), QColor('#ffbb57'), QColor('#8ea2ac'), QColor('#e67070'), QColor('#acd47e'), QColor('#ffd347'), QColor('#899ea9'), QColor('#d6d6d6'), QColor('#ffd13f'), QColor('#869ca7'), QColor('#e96665'), QColor('#8c69c9'), QColor('#869ca7'), QColor('#ff7d54'), QColor('#6b78c6'), QColor('#ff7d54'), QColor('#8298a3'), QColor('#ffce38'), QColor('#26a69a'), QColor('#22c5d9'), QColor('#a841ba'), QColor('#ffc71e'), QColor('#5363bd'), QColor('#1eb2f5'), QColor('#5cb660'), QColor('#ff6636'), QColor('#f24940'), QColor('#ff5a25'), QColor('#89c149'), QColor('#9926af'), QColor('#5e7a88'), QColor('#4aac4e'), QColor('#3d4fb2'), QColor('#ffbd05'), QColor('#47a64b'), QColor('#725044'), QColor('#1f8be8'), QColor('#fdda36'), QColor('#797979'), QColor('#ffaf00'), QColor('#78af40'), QColor('#00a8bc'), QColor('#008476'), QColor('#008375'), QColor('#74ab3e'), QColor('#b7bf2f'), QColor('#4c646f'), QColor('#0292db'), QColor('#f88400'), QColor('#fcc72f'), QColor('#fbc52f'), QColor('#0099aa'), QColor('#388e3c'), QColor('#689f38'), QColor('#adb22a'), QColor('#f47a00'), QColor('#0285cd'), QColor('#2e3c9b'), QColor('#573b33'), QColor('#ff9900'), QColor('#f37600'), QColor('#338537'), QColor('#007163'), QColor('#598f31'), QColor('#2a3795'), QColor('#008591'), QColor('#ff8f00'), QColor('#00838f'), QColor('#2a772e'), QColor('#f89c21'), QColor('#eb6100'), QColor('#3b2199'), QColor('#50158f'), QColor('#f58318'), QColor('#827717')])
        self.m_connected = random.choice([True, False])

    def setRadius(self, radius: 10):
        self.radius = radius
        self._portDrawShape: QRectF = self.computePortBoundingRect(self.targetPos)
        self._dirIndicatorDrawShape: QRectF = self.computeTriangleIndicatorBoundingRect(self.targetPos)
        self.m_dirIndicatorDrawPath: QPainterPath = self.computeTriangleIndicatorDrawPath(self.targetPos)
        self.m_boundingRect: QRectF = self.computeBoundingRect(self.targetPos, padding=0.2)

    def computeBoundingRect(self, pos: QPointF, padding: float) -> QRectF:
        pinBoundingRect: QRectF = QRectF(pos.x() - self.radius,
                                         pos.y() - self.radius,
                                         self.radius * 2,
                                         self.radius * 2)

        dirIndicatorBoundingRect: QRectF = self.m_dirIndicatorDrawPath.boundingRect()
        fontStats: QFontMetricsF = QFontMetricsF(self.m_font)
        fontWidth = fontStats.width(self._displayName) + self.radius

        bndRectWidth = dirIndicatorBoundingRect.topRight().x() - pinBoundingRect.topLeft().x()
        bndRectHeight = pinBoundingRect.height()

        if self.pinType == PinType.Input:
            return QRectF(pinBoundingRect.topLeft().x() - bndRectWidth * padding,
                          pinBoundingRect.topLeft().y() - bndRectHeight * padding,
                          bndRectWidth + fontWidth + 2 * bndRectWidth * padding,
                          bndRectHeight + 2 * bndRectHeight * padding)

        else:
            return QRectF(pinBoundingRect.topLeft().x() - bndRectWidth * padding - fontWidth,
                          pinBoundingRect.topLeft().y() - bndRectHeight * padding,
                          bndRectWidth + fontWidth + 2 * bndRectWidth * padding,
                          bndRectHeight + 2 * bndRectHeight * padding)

    # def boundingRect(self):
    #     padding = 0.2
    #     bndRectWidth = self._dirIndicatorDrawShape.topRight().x() - self._portDrawShape.topLeft().x()
    #     bndRectHeight = self._portDrawShape.height()
    #
    #     bndRect: QRectF = QRectF(self._portDrawShape.topLeft().x()-bndRectWidth * padding,
    #                              self._portDrawShape.topLeft().y()-bndRectHeight * padding,
    #                              bndRectWidth+2*bndRectWidth * padding,
    #                              bndRectHeight+2*bndRectHeight * padding)
    #
    #     return bndRect

    def boundingRect(self):
        return self.m_boundingRect

    def computePortBoundingRect(self, pos: QPointF) -> QRectF:
        return QRectF(pos.x() - self.radius,
                      pos.y() - self.radius,
                      self.radius * 2,
                      self.radius * 2)

    def computeTriangleIndicatorBoundingRect(self, pos: QPointF) -> QRectF:
        triBndRectHeight = self.radius
        triBndRectWidth = triBndRectHeight / 2
        triToPortDist = self.radius * 1.3

        return QRectF(pos.x() + triToPortDist,
                      pos.y() - triBndRectHeight / 2,
                      triBndRectWidth,
                      triBndRectHeight)

    def computeTriangleIndicatorDrawPath(self, pos: QPointF) -> QPainterPath:
        triBndRectHeight = self.radius * 1.7
        triBndRectWidth = triBndRectHeight / 1.7
        triToPortDist = self.radius * 0.6

        boundRect = QRectF(pos.x() + triToPortDist,
                           pos.y() - triBndRectHeight / 2,
                           triBndRectWidth,
                           triBndRectHeight)
        # print(boundRect.height())

        print(boundRect.right())
        w = boundRect.center().y()
        drawPath = QPainterPath(boundRect.topLeft())
        drawPath.lineTo(boundRect.right(), boundRect.center().y() )
        drawPath.lineTo(boundRect.bottomLeft())
        drawPath.closeSubpath()
        k = drawPath.boundingRect()
        return drawPath

    def paint(self, painter: QPainter, option: QStyleOptionGraphicsItem, widget=None) -> None:
        debug = False
        if debug:
            painter.drawRect(self.boundingRect())

        old_color = self.m_color
        if self.isSelected():
            self.m_color = QColor(225, 163, 0)

        pen = QPen(self.m_color, 3)

        painter.setPen(pen)
        # portDrawShape: QRectF = self.computePortBoundingRect(self.targetPos)
        # dirIndicatorDrawShape: QRectF = self.computeTriangleIndicatorBoundingRect(self.targetPos)
        # dirIndicatorDrawPath = QPainterPath(self._dirIndicatorDrawShape.topLeft())
        # dirIndicatorDrawPath.lineTo(self._dirIndicatorDrawShape.right(), self._dirIndicatorDrawShape.top() + self._dirIndicatorDrawShape.height() / 2)
        # dirIndicatorDrawPath.lineTo(self._dirIndicatorDrawShape.bottomLeft())
        # dirIndicatorDrawPath.closeSubpath()
        # painter.setBrush(self.m_color)
        # painter.setBrush(QColor(21, 23, 20))
        #
        # painter.drawEllipse(-self.radius, -self.radius, self.radius*2, self.radius*2)

        # painter.setBrush(Qt.darkRed)
        pen.setWidth(2)
        # pen.setJoinStyle(Qt.RoundJoin)
        pen.setCapStyle(Qt.RoundCap)
        painter.setPen(pen)
        painter.setBrush(self.m_color)
        painter.drawPath(self.m_dirIndicatorDrawPath)
        painter.setBrush(QColor(51, 51, 51))
        pen = QPen(self.m_color, 3)
        painter.setPen(pen)
        painter.drawEllipse(-self.radius, -self.radius, self.radius * 2, self.radius * 2)
        if self.m_connected:
            painter.setBrush(self.m_color)
            painter.drawEllipse(-self.radius * 0.5, -self.radius * 0.5, self.radius * 1, self.radius * 1)
        # textPos: QPointF = self.m_dirIndicatorDrawPath.boundingRect().bottomRight() + QPointF(self.radius, self.radius / 4)

        painter.setFont(self.m_font)
        painter.setPen(Qt.lightGray)

        if self.pinType == PinType.Input:
            textPos: QPointF = QPointF(self.radius * 2.9, self.radius / 1.3)
        else:
            textPos: QPointF = QPointF(self.m_boundingRect.left(), self.radius / 1.3)

            # textPos: QPointF = QPointF(-56-self.radius*1.3, self.radius/1.3)

        painter.drawText(textPos, self._displayName)

        if debug:
            painter.setPen(QPen(Qt.green, 1))
            painter.setBrush(Qt.transparent)

            painter.drawRect(QRectF(-self.radius, -self.radius, self.radius * 2, self.radius * 2))
            painter.drawRect(self._dirIndicatorDrawShape)

        self.m_color = old_color

    def mousePressEvent(self, event: QGraphicsSceneMouseEvent) -> None:
        if event.button() == Qt.RightButton:
            pass

        super().mousePressEvent(event)

    def contextMenuEvent(self, event: 'QGraphicsSceneContextMenuEvent') -> None:
        self.setSelected(True)

        menu = QMenu()
        menu.addAction("Pin Action 1")
        menu.addAction("Pin Action 2")
        menu.exec(event.screenPos())
        self.setSelected(False)


class NodeEditorView(QGraphicsView):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self.clickPosition = QPointF()

        self.setBackgroundBrush(QColor(53, 53, 53))

        self.setDragMode(QGraphicsView.ScrollHandDrag)
        self.setRenderHint(QPainter.Antialiasing, True)
        self.setRenderHint(QPainter.TextAntialiasing, True)
        self.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        self.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)

        self.setTransformationAnchor(QGraphicsView.AnchorUnderMouse)

        self.setCacheMode(QGraphicsView.CacheBackground)
        self.setViewportUpdateMode(QGraphicsView.BoundingRectViewportUpdate)

        self.setRenderHints(QPainter.Antialiasing | QPainter.SmoothPixmapTransform | QPainter.TextAntialiasing)

        self.rubberBandChanged.connect(self.selectionAreaChanged)

        what = QtGraphicsNodeItem(5, 5, True, "Example Node", "Math")
        what.setInputControllForPin(0, "StringInput")
        what.setInputControllForPin(1, "BoolInput")
        what.setInputControllForPin(2, "StringInput", color="gray")
        what.setInputControllForPin(3, "IntInput")
        what.setInputControllForPin(4, "")

        # what2 = QtGraphicsPin()
        # what3 = QtGraphicsExecPin()
        # what.setPos(pos)
        # what2.setPos(pos)
        # what3.setPos(pos)
        self.scene().addItem(what)

    def drawBackground(self, painter: QPainter, r: QRect):

        super().drawBackground(painter, r)

        def drawGrid(gridStep: int):
            windowRect = self.rect()
            tl = self.mapToScene(windowRect.topLeft())
            br = self.mapToScene(windowRect.bottomRight())

            left = math.floor(tl.x() / gridStep - 0.5)
            right = math.floor(br.x() / gridStep + 1.0)
            bottom = math.floor(tl.y() / gridStep - 0.5)
            top = math.floor(br.y() / gridStep + 1.0)

            # vertical lines
            for xi in range(left, right):
                line = QLine(xi * gridStep, bottom * gridStep,
                             xi * gridStep, top * gridStep)
                painter.drawLine(line)

            #       # horizontal lines
            for yi in range(bottom, top):
                line = QLine(left * gridStep, yi * gridStep,
                             right * gridStep, yi * gridStep)
                painter.drawLine(line)

        pfine = QPen(QColor(60, 60, 60), 2.0)
        painter.setPen(pfine)
        drawGrid(40)  # 20
        p = QPen(QColor(25, 25, 25), 2.0)
        painter.setPen(p)
        drawGrid(400)  # 200

    def wheelEvent(self, event: QWheelEvent):
        delta = event.angleDelta()

        def scaleUp():
            factor = math.pow(1.2, 1.0)
            t = self.transform()

            if t.m11() > 2.0:
                return
            self.scale(factor, factor)

        def scaleDown():
            factor = math.pow(1.2, -1.0)
            t = self.transform()
            if t.m11() <= 0.1:
                return
            self.scale(factor, factor)

        if delta.y() == 0:
            event.ignore()
            return

        d = delta.y() / abs(delta.y())

        if d > 0.0:
            scaleUp()
        else:
            scaleDown()

    def keyPressEvent(self, event: QKeyEvent):
        if event.key() == Qt.Key_Shift:
            self.setDragMode(QGraphicsView.RubberBandDrag)
        if event.key() == Qt.Key_Space:
            self.setSceneRect(self.scene().itemsBoundingRect())
            self.fitInView(self.scene().itemsBoundingRect(), Qt.KeepAspectRatio)

        super().keyPressEvent(event)

    def keyReleaseEvent(self, event: QKeyEvent):
        if event.key() == Qt.Key_Shift:
            self.setDragMode(QGraphicsView.ScrollHandDrag)
        super().keyPressEvent(event)

    def mousePressEvent(self, event: QMouseEvent):
        if event.button() == Qt.LeftButton:
            self.clickPosition = self.mapToScene(event.pos())

        super().mousePressEvent(event)

    #
    # def mouseReleaseEvent(self, event: QMouseEvent):
    #     if event.button() == Qt.LeftButton and event.modifiers() == Qt.ShiftModifier:
    #         menu = QMenu()
    #         sortalphabeticalDescending = QAction("By Letter [A to Z]", self)
    #         menu.addAction(sortalphabeticalDescending)
    #         menu.exec_(self.viewport().mapToGlobal(event.pos()))
    #
    #     super().mouseReleaseEvent(event)

    def mouseMoveEvent(self, event: QMouseEvent):
        if self.scene().mouseGrabberItem() == None and event.buttons() == Qt.LeftButton:
            if event.modifiers() != Qt.ShiftModifier:
                diff = self.clickPosition - self.mapToScene(event.pos())
                self.setSceneRect(self.sceneRect().translated(diff.x(), diff.y()))
        super().mouseMoveEvent(event)

    def c1c2point(self, _out: QPointF):
        defaultOffset = 200

        _in = QPointF(30, 40)

        xDistance = _in.x() - _out.x()

        horizontalOffset = min(defaultOffset, abs(xDistance))

        verticalOffset = 0

        ratioX = 0.5

        if xDistance <= 0:
            yDistance = _in.y() - _out.y() + 20

            if yDistance < 0:
                vector = -1.0
            else:
                vector = 1.0
            verticalOffset = min(defaultOffset, abs(yDistance)) * vector
            ratioX = 1.0

        horizontalOffset = horizontalOffset * ratioX

        c1 = QPointF(_out.x() + horizontalOffset,
                     _out.y() + verticalOffset)

        c2 = QPointF(_in.x() - horizontalOffset,
                     _in.y() - verticalOffset)

        return c1, c2

    def contextMenuEvent(self, event: QContextMenuEvent):
        pos: QPointF = self.mapToScene(event.pos())

        if self.itemAt(event.pos()):
            super().contextMenuEvent(event)
            return

        #
        # if self.items(QRectF):
        #     super().contextMenuEvent(event)
        #     return

        scene: QGraphicsScene = self.scene()

        # if self.scene().items(QRectF(pos.x(), pos.y(), 50, 50)):
        #     super().contextMenuEvent(event)
        #     QApplication.setOverrideCursor(Qt.ForbiddenCursor)
        #     QTimer.singleShot(200, QApplication.restoreOverrideCursor)
        #     return

        rect = RectItem(pos.x(), pos.y(), 50, 50)
        # whatever = RectItem(pos.x() +5, pos.y()+5, 10, 10)
        # whatever.setParentItem(rect)
        # scene.addItem(rect)

        what = QtGraphicsNodeItem(3, 5, True, "Example Node", "Math")
        what.setInputControllForPin(0, "StringInput")
        what.setInputControllForPin(1, "BoolInput")
        what.setInputControllForPin(2, "")
        constant = ConstantNode()

        # what2 = QtGraphicsPin()
        # what3 = QtGraphicsExecPin()
        # what.setPos(pos)
        # what2.setPos(pos)
        # what3.setPos(pos)
        scene.addItem(what)
        scene.addItem(constant)
        # scene.addItem(what2)
        # scene.addItem(what3)

        # return
        #

        path = QPainterPath(QPointF(30, 40))

        #####

        pathItem = QGraphicsPathItem()
        pathItem.setFlag(QGraphicsPathItem.ItemIsSelectable)
        pathItem.setFlag(QGraphicsPathItem.ItemIsMovable)

        #
        self.sourcePos = QPointF(30, 40)
        self.targetPos = QPointF(pos.x(), pos.y())
        path.moveTo(self.sourcePos)

        self.curv1 = 1
        self.curv2 = 0.1

        self.curv3 = 0.1
        self.curv4 = 1

        dx = self.targetPos.x() - self.sourcePos.x()
        dy = self.targetPos.y() - self.sourcePos.y()

        p1 = self.sourcePos
        p2 = self.targetPos
        multiply = 3

        # ctrl1 = QPointF(self.sourcePos.x() + dx * self.curv1,
        #                        self.sourcePos.y() + dy * self.curv2)
        # ctrl2 = QPointF(self.sourcePos.x() + dx * self.curv3,
        #                        self.sourcePos.y() + dy * self.curv4)
        xDistance = self.targetPos.x() - self.sourcePos.x()
        if xDistance < 0:
            path.cubicTo(QPointF(p1.x() + xDistance / -multiply, p1.y()),
                         QPointF(p2.x() - xDistance / -multiply, p2.y()), p2)
        else:
            path.cubicTo(QPointF(p1.x() + xDistance / multiply, p1.y()), QPointF(p2.x() - xDistance / 2, p2.y()), p2)

            # path.cubicTo(QtCore.QPoint(p1.x() + distance / multiply, p1.y()),
            #              QtCore.QPoint(p2.x() - distance / 2, p2.y()), p2)

        # path.cubicTo(ctrl1, ctrl2, self.targetPos)

        # ctrl1.setY(self.sourcePos.y())
        # ctrl2.setY(self.targetPos.y())
        #
        # ctrl2.setX(self.sourcePos.x())
        # ctrl1.setX(self.targetPos.x())
        # path.cubicTo(ctrl1, ctrl2, self.targetPos)

        pathItem.setPath(path)
        # pad = QPainterPath(ctrl1)
        # pad.lineTo(ctrl2)

        ###

        # scene.addItem(pathItem)
        # scene.addItem(QGraphicsPathItem(pad))

        # scene.addItem(rect)

    def selectionAreaChanged(self, areaRect: QRect, fromScenePoint: QPointF, toScenePoint: QPointF):

        #     scene: QGraphicsScene = self.scene()
        #     # selectionArea: QPainterPath = QPainterPath()
        #
        #     #
        #     # for item in scene.items(QPolygonF(self.mapToScene(areaRect))):
        #     #     item.setSelected(True)
        #     #
        #     # # selectionArea.addPolygon(QPolygonF(self.mapToScene(areaRect)))
        #     # # R.closeSubpath()
        #     # # scene.setSelectionArea(selectionArea)
        #     # #
        #     # #
        #     # #
        print(self.scene().selectedItems())


class ScriptVariableTable(QTableWidget):
    def __init__(self, var_types: List[Tuple[str, QColor]], **kwargs):
        super().__init__(**kwargs)

        # self.setContentsMargins(30,30,30,30)
        self.setRowCount(0)
        self.setColumnCount(2)
        self.setShowGrid(False)
        self.horizontalHeader().hide()
        self.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        self.verticalHeader().hide()
        self.setSelectionMode(QAbstractItemView.SingleSelection)
        self.setDragEnabled(True)
        self.setDragDropMode(QAbstractItemView.DragOnly)

        font = QApplication.font()
        font.setPointSize(10)
        self.setFont(font)


        self. var_types = var_types

    def _createTypeComboBox(self):
        cb = QComboBox()
        cb.setIconSize(QSize(12, 6))

        font = QApplication.font()
        font.setPointSize(8)
        cb.setFont(font)

        for typeName, color in self.var_types:
            cb.addItem(self.create_variable_icon(color), typeName)

        return cb

    def addVariable(self, name: str, type: str):
        row = self.rowCount()
        self.insertRow(row)

        varNameItem = QTableWidgetItem(name)
        self.setItem(row, 0, varNameItem)

        dummyItem = QTableWidgetItem()
        dummyItem.setFlags(dummyItem.flags() & ~Qt.ItemIsSelectable)

        typeComboBox = self._createTypeComboBox()
        typeComboBox.setCurrentText(type)

        self.setItem(row, 1, dummyItem)
        self.setCellWidget(row, 1, typeComboBox)

        self.resizeRowsToContents()




        # type_item = QTableWidgetItem(self.create_variable_icon(color), type)
        # type_item.setFlags(type_item.flags() & ~Qt.ItemIsEditable)
        # self.setItem(row, 1, type_item)

    @staticmethod
    def create_variable_icon(color):
        # Create a rounded version of the original pixmap

        pixmap = QPixmap(100, 50)

        mask = QBitmap((pixmap.size()))
        mask.fill(Qt.color0)

        painter = QPainter(mask)
        painter.setRenderHint(QPainter.Antialiasing)
        painter.setBrush(Qt.color1)
        painter.drawRoundedRect(mask.rect(), 20, 20)
        painter.end()

        pixmap.fill(color)
        pixmap.setMask(mask)

        return QIcon(pixmap)


class LeftSidePanel(QWidget):
    def __init__(self, parent):
        super().__init__(parent)

        self.parent = parent

        dock = QDockWidget("Collapsible Demo")
        font = QApplication.font()
        font.setBold(True)

        dock.setFont(font)
        parent.addDockWidget(Qt.LeftDockWidgetArea, dock)
        scroll = QScrollArea()

        tab = QTabWidget()
        tab.addTab(scroll, "Script")

        dock.setWidget(tab)

        content = QWidget()
        content.setStyleSheet("""
            QWidget
            {
                    background-color: rgb(26,26,26);        
            }
            
            QPushButton
            {
                background-color: rgb(56, 56, 56);
                border-radius: 4px;
                border: 1px solid black;
                padding: 4px;

            }
            
            QPushButton:pressed
            { 
                background-color: rgb(47, 47, 47); 
            }
            
            QPushButton:hover:!pressed
            {
                background-color: rgb(87, 87, 87);
            }
            
            QTableWidget 
            {
                border: none;
            }   
            
            QComboBox 
            {
                selection-background-color: rgb(56, 56, 56);
                background-color: rgb(19,19,19);
                padding-left: 4px;
                padding-right: 4px;
            }
        
            """)

        scroll.setWidget(content)
        scroll.setWidgetResizable(True)
        vlay = QVBoxLayout(content)
        vlay.setSpacing(2)
        vlay.setContentsMargins(4, 4, 4, 4)

        button_and_search_bar_layout = QHBoxLayout()

        add_button = QPushButton(
            QIcon(r"C:\Users\ruwen\OneDrive\Desktop\nodeflow\nodeflow-gui\icons\icon_Blueprint_AddFunction_512px.png"),
            "Add")
        font_ = QApplication.font()
        font_.setPointSize(10)
        add_button.setFont(font_)
        add_button.setFixedWidth(70)

        search_bar = QLineEdit()
        search_bar.setClearButtonEnabled(True)
        search_bar.setFont(font_)
        search_bar.setPlaceholderText("Search")
        search_bar.setStyleSheet("""
                QLineEdit
                {
                    background-color: rgb(15, 15, 15);
                    border: 1px solid rgb(53, 53, 53);
                    border-radius: 9px;
                    padding: 1px;
                    spacing: 0px;
                    margin: 0px;
                }
                
                QLineEdit:focus
                {
                    border: 1px solid rgb(6, 100, 195);
                }
                
        """)

        search_bar.addAction(QIcon(r"C:\Users\ruwen\OneDrive\Desktop\nodeflow\nodeflow-editor\icons\find.svg"),
                             QLineEdit.LeadingPosition)

        button_and_search_bar_layout.addWidget(add_button)
        button_and_search_bar_layout.addWidget(search_bar)

        tab.tabBar().setFont(font_)

        vlay.addLayout(button_and_search_bar_layout)

        for i in range(2):
            box = CollapsibleBox(["FUNCTIONS", "MACROS"][i])
            vlay.addWidget(box)
            lay = QVBoxLayout()
            for j in range(10):
                label = QLabel("{}".format(j))
                color = QColor(*[random.randint(0, 255) for _ in range(3)])
                label.setStyleSheet(
                    "background-color: {}; color : white;".format(color.name())
                )
                label.setAlignment(Qt.AlignCenter)
                lay.addWidget(label)

            box.setContentLayout(lay)

        box = CollapsibleBox("VARIABLES")
        vlay.addWidget(box)
        lay = QVBoxLayout()

        variable_table = ScriptVariableTable([
            ("Boolean", QColor("#950000")),
            ("Byte", QColor("#006f65")),
            ("Integer", QColor("#1fe3af")),
            ("Float", QColor("#38d500")),
            ("String", QColor("#ff00d4")),
            ("Vector", QColor("#ffca23")),
            ("Rotator", QColor("#a0b4ff")),
            ("Transform", QColor("#ff7300")),
        ])


        data = [
            ["NewVar", "Boolean"],
            ["NewVar_0", "Byte"],
            ["NewVar_1", "Integer"],
            ["NewVar_2", "Float"],
            ["NewVar_3", "String"],
            ["NewVar_4", "Vector"],
            ["NewVar_5", "Rotator"],
            ["NewVar_6", "Transform"]
        ]
        print(variable_table.height())

        for row in range(len(data)):
            variable_table.addVariable(name=data[row][0], type=data[row][1])

        lay.addWidget(variable_table)
        box.setContentLayout(lay)

        vlay.addStretch()


class RightSidePanel(QWidget):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        layout = QVBoxLayout(self)
        widgets = [
            QCheckBox,
            QComboBox,
            QDateEdit,
            QDateTimeEdit,
            QDial,
            QDoubleSpinBox,
            QFontComboBox,
            QLCDNumber,
            QLabel,
            QLineEdit,
            QProgressBar,
            QPushButton,
            QRadioButton,
            QSlider,
            QSpinBox,
            QTimeEdit,
        ]

        for w in widgets:
            layout.addWidget(w())


class Editor(QMainWindow):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self.setup()
        self.setWindowState(Qt.WindowMaximized)

    def setup(self):
        self.editorWidget = QSplitter()
        self.editorWidget.setOrientation(Qt.Horizontal)

        self.centralLayout = QHBoxLayout(self)

        self.leftSidePanelArea = QScrollArea()
        self.leftSidePanelArea.setWidget(LeftSidePanel(self))

        self.scene = QGraphicsScene(self)
        self.view = NodeEditorView(self.scene)

        self.rightSidePanelArea = QScrollArea()
        self.rightSidePanelArea.setWidget(RightSidePanel())

        self.editorWidget.addWidget(self.leftSidePanelArea)
        self.editorWidget.addWidget(self.view)

        # self.centralLayout.addWidget(self.leftSidePanelArea)
        # self.centralLayout.addWidget(self.view)
        # self.centralLayout.addWidget(self.rightSidePanelArea)

        # splitter.setLayout(self.centralLayout)

        self.setCentralWidget(self.editorWidget)

        self.addToolBar(QToolBar())

        self.smartResizeSplitterContent()

    def smartResizeSplitterContent(self):
        availWidth = QApplication.desktop().screenGeometry().width()

        # Resize Panels to recommended size and let the view take the rest of the screen space
        leftSidePanelWidth = self.leftSidePanelArea.sizeHint().width()
        rightSidePanelWidth = self.rightSidePanelArea.sizeHint().width()
        sceneViewWidth = availWidth - leftSidePanelWidth - rightSidePanelWidth

        self.editorWidget.setSizes([leftSidePanelWidth, sceneViewWidth, rightSidePanelWidth])

        self.leftSidePanelArea.setMinimumWidth(leftSidePanelWidth)
        self.rightSidePanelArea.setMinimumWidth(rightSidePanelWidth)


def setStyleMode(mode: str):
    palette = QPalette()

    if mode == "daymode":
        palette = QPalette()
        palette.setColor(QPalette.HighlightedText, Qt.white)
        palette.setColor(QPalette.Highlight, QColor(0, 85, 127))

    elif mode == "darkmode":
        palette.setColor(QPalette.Window, QColor(60, 63, 65))
        palette.setColor(QPalette.WindowText, Qt.lightGray)
        palette.setColor(QPalette.Base, QColor(53, 53, 53))
        palette.setColor(QPalette.AlternateBase, QColor(53, 53, 53))
        palette.setColor(QPalette.ToolTipBase, Qt.black)
        palette.setColor(QPalette.ToolTipText, Qt.white)
        palette.setColor(QPalette.Text, Qt.lightGray)
        palette.setColor(QPalette.Button, QColor(53, 53, 53))
        palette.setColor(QPalette.ButtonText, Qt.lightGray)
        palette.setColor(QPalette.BrightText, Qt.white)
        palette.setColor(QPalette.Link, QColor(42, 130, 218))
        palette.setColor(QPalette.Highlight, QColor(6, 100, 195))
        palette.setColor(QPalette.HighlightedText, Qt.white)

        app.setStyleSheet("""
             QMenu
             {
                 background-color: rgb(56, 56, 56);
                 color: white;
                 font-size: 10pt;
             }
             QMenu::item:selected
             {
                 background-color: rgb(6, 100, 195);
                 color: rgb(255, 255, 255);

             }
             QMenu:item:disabled
             {
                 color: gray;
             }
             
             """)

        #
        # palette.setColor(QPalette.Disabled, QPalette.Button, Qt.darkGray)
        palette.setColor(QPalette.Disabled, QPalette.ButtonText, Qt.darkGray)
        # palette.setColor(QPalette.Disabled, QPalette.WindowText, Qt.lightGray)
        # palette.setColor(QPalette.Disabled, QPalette.WindowText, Qt.lightGray)
        # palette.setColor(QPalette.Disabled, QPalette.Text, QColor(120, 120, 120))
        # palette.setColor(QPalette.Disabled, QPalette.Light, Qt.transparent)

    app.setPalette(palette)


if __name__ == '__main__':
    # import SecretColors
    #
    # palette = SecretColors.Palette("material")
    # print(palette.blue())
    # #F k = palette.random(no_of_colors=100)
    # #
    # # colors = [f"QColor('{color}')" for color in k]
    # # print(colors)

    app = QApplication([])
    app.setStyle("fusion")
    setStyleMode("darkmode")

    id = QFontDatabase.addApplicationFont(r"../OpenSans-Regular.ttf")
    family = QFontDatabase.applicationFontFamilies(id)[(0)]
    font = QFont("Roboto")
    font.setHintingPreference(QFont.HintingPreference.PreferNoHinting)
    app.setFont(font)




    window = Editor()
    window.show()
    # window.setGeometry(app.desktop().screenGeometry(1))

    status = app.exec_()
    sys.exit(status)
