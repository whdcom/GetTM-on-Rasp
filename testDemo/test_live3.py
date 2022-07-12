import math
import os

import cv2
import matplotlib.pyplot as plt
import  numpy as np

#获取二值图像，对于温度低于27度像素点置为0，高于置为255
def get_binary_img(content):#得到温度矩阵的字符串列表

    content = list(content.split(","))
    image = np.array(content, np.float32)

    gray = np.zeros((192, 256), np.uint8).flatten()
    for i, v in enumerate(image):
        if v > temptrue_thresh:
            gray[i] = 255
    image = gray.reshape((192, 256))

    cnts, hierarchy = cv2.findContours(image, cv2.RETR_EXTERNAL,
                                       cv2.CHAIN_APPROX_SIMPLE)

    image = cv2.merge([image, image, image])


    # for c in cnts:
    #     area = cv2.contourArea(c, False)
    #     #x, y, w, h = cv2.boundingRect(c)
    #     if area < 1000:
    #         cv2.drawContours(image, [c], -1, (0, 0, 0), -1)#开始去除小面积区域。
    #         # mage[x:x + w, y:y + h,:] = 0  # 消除小面积的噪声
    cnts=sorted(cnts, key=lambda x: cv2.contourArea(x, False), reverse=True)
    for i,c in enumerate(cnts[1:]):
        #area = cv2.contourArea(c, False)
        # print("cnt[",i,"]area:",area)
        #if area < 1000:
        cv2.drawContours(image, [c], -1, (0, 0, 0), -1)#开始去除小面积区域。


    image = image[:, :, 0]
    return image#返回二值图像。

#返回轮廓中心的元组。
def get_img_center(binary_image):
    # gray = gray_img
    # kernel = np.ones((3, 3), np.uint8)  # 这个kernel要构建出来，kernel越大，腐蚀得也越多
    # image = cv2.morphologyEx(gray, op=cv2.MORPH_OPEN, kernel=kernel, iterations=2)
    image=binary_image
    cnts, hierarchy = cv2.findContours(image, cv2.RETR_EXTERNAL,
                                       cv2.CHAIN_APPROX_SIMPLE)
    if len(cnts)>0:#有些照片有问题，没有目标。
        M = cv2.moments(cnts[0])#因为是单目标，这里去掉循环，直接用第一个
        cX = int(M["m10"] / M["m00"])
        cY = int(M["m01"] / M["m00"])
        return (cX, cY)
    else:
        return None


#raw_img获得的温度原始图像[w,h,1]，
#binary 二值图像
#返回中心点，如果没有目标，返回None
def detect_live(raw_img,binary):

    center_color=(255,255,0)#中心点，黄色
    show_raw = True


    center=get_img_center(binary)
    if center is not  None:#有目标的时候

        #cv2.namedWindow("img", 0)
        if show_raw==False:
            img_copy=binary.copy()
            img_copy=cv2.merge([img_copy,img_copy,img_copy])#合并通道，以便，画出彩色图像，否则画不出彩色图像
            cv2.circle(img_copy, center, 1, center_color, 2)  #

            #cv2.circle(img_copy, first_center, dead_radius, live_circle_color, 2)
            #cv2.imshow("img", img_copy)
        else:
            cv2.circle(raw_img, center, 1, center_color, 2)#
            #cv2.circle(raw_img, first_center, dead_radius, live_circle_color, 2)
            #cv2.imshow("img",raw_img)
        #cv2.waitKey(1)
        return center
    return None

#返回原始温度图像
def get_raw_img(content):

    content = list(content.split(","))
    image = np.array(content, np.float32).reshape((192,256))
    image=np.uint8((image-np.min(image))*255/np.max(image))
    #image = np.uint8((image - np.min(image)) * 255 / np.max(image))
    image=cv2.merge([image,image,image])
    #cv2.imshow("image",image)
    #cv2.waitKey(0)
    return image

#会返回中心点，有可能为None
def detect_one_frame(content):
    '''
    :param content: 温度矩阵字符串
    :return: 会返回中心点，有可能为None
    '''
    raw_img=get_raw_img(content)
    binary=get_binary_img(content)
    return detect_live(raw_img,binary)#返回中心点。




center_records=[]#中心点的list
record_map={}#中心点缩小之后的map
dead_radius = 12
temptrue_thresh=30

def setDeadRadius(radius):
    '''

    :param radius: 死亡半径
    :return: 设置死亡半径
    '''
    global dead_radius
    dead_radius=radius

def setTemptureThresh(tempture):
    global temptrue_thresh
    temptrue_thresh=tempture
#total 总的帧数
#center 中心点的元组
#last_frame 最后一帧的温度图片,如果是最后一帧的时候，就要传进last_frame,即last_frame不为None
def judgingDeadOrLive(center):
    # if center is None:
    #     return


    cx, cy = center
    center_records.append(center)
    shrink_center_key = (int(cx / dead_radius), int(cy / dead_radius))

    if shrink_center_key in record_map:
        records = record_map[shrink_center_key]
        records.append([cx, cy])
    else:
        record_map[shrink_center_key] = [[cx, cy]]


def judge_in_one_circle(centers):
    '''

    :param centers: 中心点的列表
    :return:in_one_circle,(avgX,avgY),是否在同属于一个均值中心点，和均值中心点
    '''
    totalX=0
    totalY=0
    for cx,cy in centers:
        totalX=totalX+cx
        totalY=totalY+cy
    avgX=int(totalX/len(centers))
    avgY = int(totalY / len(centers))
    in_one_circle=True
    for cx,cy in centers:
        if (math.sqrt((cx-avgX)**2 +(cy-avgY)**2))>dead_radius:
            in_one_circle=False
            break
    return in_one_circle,(avgX,avgY)
def getJudgeResult(total, last_center, last_frame):
    is_live = True
    live_circle_color = (0, 255, 0)  # 活的绿色
    dead_circle_color = (0, 0, 255)  # 死的红
    center_color = (255, 255, 0)  # 中心点，黄色

    cx, cy = last_center
    shrink_center_key = (int(cx / dead_radius), int(cy / dead_radius))
    if last_frame is not None:  # 说明是最后一帧了
        #last_frame = cv2.merge([last_frame, last_frame, last_frame])  # 合并通道
        for i,c in enumerate(center_records):
            cv2.circle(last_frame, c, 1, center_color, 2)  # 画出所有的鸡的点。
            if i>0:
                cv2.line(last_frame,center_records[i-1],center_records[i],center_color,1)



        # if shrink_center_key in record_map and total == len(record_map[shrink_center_key]):#判活
        #     is_live = False
        # else:
        #     print(record_map[shrink_center_key])
        dead,avg_center=judge_in_one_circle(center_records)
        if dead:
            cv2.circle(last_frame, avg_center, dead_radius, dead_circle_color, 2)  # 画出最后一帧的鸡的圈
        else:
            cv2.circle(last_frame, last_center, dead_radius, live_circle_color, 2)  # 画出最后一帧的鸡的圈,活的

    record_map.clear()#清空，为下一次准备
    center_records.clear()
    return last_frame, dead  # 返回最后一帧的检测结果rgb图像和存活结果。






