# STM32F4 SysTick Timer (Without STK_CALIB)

## Description

This repository demonstrates how to configure the SysTick timer on STM32F4 microcontrollers without using the STK_CALIB register. The project provides an example of generating periodic interrupts and measuring time intervals directly through SysTick configuration, making it ideal for low-level embedded applications.

## Features

- Configure SysTick timer without relying on STK_CALIB
- Generate periodic interrupts for time-based tasks
- Handle SysTick interrupt in a simple example
- Easy-to-understand C code suitable for STM32F4 beginners and hobbyists

## NOTES

1. T(SysTick timer counter) sn = (STK_LOAD+1)/F(SysTick timer counter)Hz => Period = 1 / Frequency
2. STK_LOAD can be max 16.777.215

## Documentions

1. STM32F407 Reference manual
2. STM32 Programming manual

