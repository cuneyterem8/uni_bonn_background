library(stats)
library(XNomial)
library(car)
library(coin)
library(plyr)
library(multcomp)

#data preperation

data = read.csv("C:/Users/cnytC/Desktop/testdoc/survey1Data.csv", stringsAsFactors = FALSE)

data = data[data$Q1=="I consent",]
data = data[data$DistributionChannel == "test",]
data = data[data$Progress == 100,]

#Hypotesis: Most probably users having less experience about social media have less concerns about security
#Q4 shows experience of using social media and Q3 shows academic background 

expdata = data

expdata$Q4[expdata$Q4 == "more than 6 months"] <- "have experience"
expdata$Q4[expdata$Q4 == "less than 6 months"] <- "no experience"

expdata$Q3[expdata$Q3 == "Professional degree"] <- "higher education"
expdata$Q3[expdata$Q3 == "Doctorate"] <- "higher education"
expdata$Q3[expdata$Q3 == "4 year degree"] <- "higher education"
expdata$Q3[expdata$Q3 == "2 year degree"] <- "lower education"
expdata$Q3[expdata$Q3 == "High school graduate"] <- "lower education"
expdata$Q3[expdata$Q3 == "Less than high school"] <- "lower education"
expdata$Q3[expdata$Q3 == "Some college"] <- "lower education"

#Q8_1 shows company trustworthy, Q31 and Q32 shows how users trust on security
expdata = expdata[expdata$Q8_1 %in% c("I trust", "I do not trust"),]
expdata = expdata[expdata$Q31 %in% c("It is secure enough", "it is not secure enough"),]
expdata = expdata[expdata$Q32 %in% c("It is secure enough", "it is not secure enough"),]

#this is results to show that users have less knowledge have less security concerns because they trust more on companies
#also they believe in that companies or security levels are high, however there are some security vulnerabilities that
#many times new vulnerabilities existed for multiple times, therefore having higher educational or using applications more
#users have more concerns about security
expdata$Q4 = factor(expdata$Q4)
trust1 = xtabs(~ Q8_1 + Q4, data = expdata)
trust1

secure1a = xtabs(~ Q31 + Q4, data = expdata)
secure1a

secure1b = xtabs(~ Q32 + Q4, data = expdata)
secure1b

expdata$Q3 = factor(expdata$Q3)
trust2 = xtabs(~ Q8_1 + Q3, data = expdata)
trust2

secure2a = xtabs(~ Q31 + Q3, data = expdata)
secure2a

secure2b = xtabs(~ Q32 + Q3, data = expdata)
secure2b

#this is the first test for fisher
fisher.test(trust1)
fisher.test(secure1a)
fisher.test(secure1b)
fisher.test(trust2)
fisher.test(secure2a)
fisher.test(secure2b)


expdata2 = data

expdata2$Q4[expdata2$Q4 == "more than 6 months"] <- 1
expdata2$Q4[expdata2$Q4 == "less than 6 months"] <- 0

expdata2$Q4 = factor(expdata2$Q4)

expdata2 = expdata2[expdata2$Q31_DO_1 != "",]
expdata2$Q31_DO_1 = as.numeric(expdata2$Q31_DO_1)

#this is the second test since i´hypotesis is one tailed and p>0.05
t.test(expdata2$Q31_DO_1 ~ expdata2$Q4, equal = TRUE)

