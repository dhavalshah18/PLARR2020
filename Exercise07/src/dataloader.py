#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Jun 10 19:31:08 2020

@author: sc
"""
import os
import cv2
import torch
import torchvision
import torchvision.transforms as transforms
# from torch.utils.data import DataLoader
from PIL import Image

class Dataset(torch.utils.data.Dataset):
    def __init__(self, input_list:list, gt_list:list):
        super().__init__()            
        self.input_list = [os.path.abspath(i) for i in input_list]
        self.gt_list    = [os.path.abspath(i) for i in gt_list]
        self.input_list.sort()
        self.gt_list.sort()
        
        self.transform = transforms.Compose([
            transforms.ToTensor(),
        ])
        
        # check
        assert(len(input_list) == len(gt_list))
        for i in range(len(input_list)):
            assert( os.path.splitext(os.path.basename(input_list[i]))[0] == os.path.splitext(os.path.basename(gt_list[i]))[0])
        
        
    def __len__(self):
        return len(self.input_list)

    def __getitem__(self, index):
        try:
            item = self.load_item(index)
        except:
            print('loading error: ' + self.data[index][0])
            item = self.load_item(0)

        return item

    def load_item(self, index):
        x = cv2.imread(self.input_list[index],cv2.IMREAD_COLOR) 
        y = cv2.imread(self.gt_list[index],cv2.IMREAD_COLOR) 
        x = cv2.cvtColor(x, cv2.COLOR_BGR2RGB)
        y = cv2.cvtColor(y, cv2.COLOR_BGR2RGB)
        
        x = Image.fromarray(x)
        y = Image.fromarray(y)
        
        
        if self.transform:
            x= self.transform(x).float()
            y= self.transform(y).float()
        
        sample = {'input': x, 'gt': y}
        return sample
        
