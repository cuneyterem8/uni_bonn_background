
options(scipen=999)
if(!require(grid)){install.packages("grid")}
if(!require(gridExtra)){install.packages("gridExtra")}

if(!require(psych)){install.packages("psych")} # when installing this package try saying "no" to compiling since compiling seems to be broken
if(!require(likert)){install.packages("likert")}
if(!require(tidyverse)){install.packages("tidyverse")}
if(!require(labelled)){install.packages("labelled")}
if(!require(ggplot2)){install.packages("ggplot2")}

if(!require(stats)){install.packages("stats")}
if(!require(XNomial)){install.packages("XNomial")}
if(!require(car)){install.packages("car")}
if(!require(coin)){install.packages("coin")}
if(!require(plyr)){install.packages("plyr")}
if(!require(multcomp)){install.packages("multcomp")}

if(!require(sjPlot)){install.packages("sjPlot")}


library(haven)
library(plyr) # warning count is defined in multiple libaries. we use the one from plyr


factors_y_axis_max=100
factors_y_axis_stepsize=10

#load data
q <- read.csv("~/sciebo/USECAP/Studies/2020/COVID-App/Qualtrics/Qualtrics2-char-order.csv")
qn <- read.csv("~/sciebo/USECAP/Studies/2020/COVID-App/Qualtrics/Qualtrics2-num-order.csv")
#qs <- read_sav("~/sciebo/USECAP/Studies/2020/COVID-App/Qualtrics/Qualtrics2-spss-order.sav")

#q <- read.csv("~/sciebo/USECAP/Studies/2020/COVID-App/CS/CS-char.csv")
#qn <- read.csv("~/sciebo/USECAP/Studies/2020/COVID-App/CS/CS-num.csv")

#q <- read.csv("~/sciebo/USECAP/Studies/2020/COVID-App/Clickworker/Clickworker-char.csv")
#qn <- read.csv("~/sciebo/USECAP/Studies/2020/COVID-App/Clickworker/Clickworker-num.csv")


#remove non-completes

qn=qn[q$Progress==100,]
q=q[q$Progress==100,]






#return vector mask of install undecideds and nos
installNoUnVec = function(x){
  xx = (x$app_install=="Auf keinen Fall nutzen" | x$app_install=="Wahrscheinlich nicht nutzen" | x$app_install=="Ich bin unentschieden")
  return(xx)
}

#return vector mask of install undecideds 
installYesUnVec = function(x){
  xx = (x$app_install=="Auf jeden Fall nutzen" | x$app_install=="Wahrscheinlich nutzen" | x$app_install=="Ich bin unentschieden")
  return(xx)
}

#return vector mask of install no
installNoVec = function(x){
  xx = (x$app_install=="Auf keinen Fall nutzen" | x$app_install=="Wahrscheinlich nicht nutzen")
  return(xx)
}

#return vector mask of install yes
installYesVec = function(x){
  xx = (x$app_install=="Auf jeden Fall nutzen" | x$app_install=="Wahrscheinlich nutzen")
  return(xx)
}

#return vector mask of yes
yesVec = function(x){
  
  xx = (x=="Eher nutzen"|x=="Auf jeden Fall nutzen") 
  return(xx)
}

#return vector mask of nos
noVec = function(x){
  
  xx = (x=="Eher nicht nutzen"|x=="Auf keinen Fall nutzen") 
  return(xx)
}


#return count of nos
no = function(y){
  
  yy = sum(y=="Eher nicht nutzen",na.rm=T) + sum(y=="Auf keinen Fall nutzen",na.rm=T) 
  
  return(yy)
}

# return sum of yeses
yes = function(x){
  xx = sum(x=="Eher nutzen",na.rm=T)+sum(x=="Auf jeden Fall nutzen",na.rm=T) 
  return(xx)
}


#return count of nos
installNo = function(y){
  
  yy = sum(y=="Wahrscheinlich nicht nutzen",na.rm=T) + sum(y=="Auf keinen Fall nutzen",na.rm=T) 
  
  return(yy)
}

# return sum of yeses
installYes = function(x){
  xx = sum(x=="Wahrscheinlich nutzen",na.rm=T)+sum(x=="Auf jeden Fall nutzen",na.rm=T) 
  return(xx)
}

#this was superseaded by plchangeMS2
#calculate whether a set of app features would change a participants opinion
#even a weak feature can overide a strong initial opinion. 
#probably only makes sense if we sum up the weak features
polchangeMS = function(ds, columns){
  #print("Calculating Polchanges:")
  ret=data.frame(ds$app_install)
  ret[,"change"] = "no change"
  
  for(i in 1:length(q$app_install)) {
    for(j in 1:length(columns)) {
      
      
      #postive outset
      if(ds$app_install[i] == "Auf jeden Fall nutzen" | ds$app_install[i] == "Wahrscheinlich nutzen"){#1
        if(ds[i,columns[j]] == "Auf keinen Fall nutzen" | ds[i,columns[j]] == "Eher nicht nutzen"){#2
          ret[i,"change"] = "negative"
        }#2
      }#1
      
      
      # negative outset
      if(ds$app_install[i] == "Auf keinen Fall nutzen" | ds$app_install[i] == "Wahrscheinlich nicht nutzen"){#3
        if(ds[i,columns[j]] == "Auf jeden Fall nutzen" | ds[i,columns[j]] == "Eher nutzen"){#4
          if(ret[i,"change"]!="no change no override"){#5
            ret[i,"change"] = "positive"
          }#5
        }#4
        else{#6
          if(ds[i,columns[j]] == "Auf keinen Fall nutzen" | ds[i,columns[j]] == "Eher nicht nutzen"){#7
            ret[i,"change"] = "no change no override"
          }#7  
        }#6
      }#3
      
      
      
      #neutral outset
      if(ds$app_install[i] == "Ich bin unentschieden"){#3 #todo Weiß ich nicht hinzufügen. Das ß macht encoding probleme
        if(ds[i,columns[j]] == "Auf jeden Fall nutzen" | ds[i,columns[j]] == "Eher nutzen"){#4
          if(ret[i,"change"]!="negative no override"){#5
            ret[i,"change"] = "positive"
          }#5
        }#4
        else{#6
          if(ds[i,columns[j]] == "Auf keinen Fall nutzen" | ds[i,columns[j]] == "Eher nicht nutzen"){#7
            ret[i,"change"] = "negative no override"
          }#7  
        }#6
      }#3
      
      
    }#for j
  }#for i
  
  for(i in 1:length(q$app_install)) {
    if(ret[i,"change"] == "negative no override"){
      ret[i,"change"] = "negative"
    }
    if(ret[i,"change"] == "no change no override"){
      ret[i,"change"] = "no change"
    }
  }
  
  
  return(ret)
}


#calculate whether a set of app features would change a participants opinion
# strong opinions can only be overridden by strong opinions 
polchangeMS2 = function(ds, columns){
  #print("Calculating Polchanges:")
  ret=data.frame(ds$app_install)
  ret[,"change"] = "no change"
  
  for(i in 1:length(q$app_install)) {
    for(j in 1:length(columns)) {
      
      
      #very postive outset
      if(ds$app_install[i] == "Auf jeden Fall nutzen"){#1
        if(ds[i,columns[j]] == "Auf keinen Fall nutzen"){#2
          ret[i,"change"] = "negative"
        }#2
      }#1
      
      #postive outset
      if(ds$app_install[i] == "Wahrscheinlich nutzen"){#1
        if(ds[i,columns[j]] == "Auf keinen Fall nutzen" | ds[i,columns[j]] == "Eher nicht nutzen"){#2
          ret[i,"change"] = "negative"
        }#2
      }#1
      
      
      # very negative outset
      if(ds$app_install[i] == "Auf keinen Fall nutzen"){#3
        if(ds[i,columns[j]] == "Auf jeden Fall nutzen"){#4
          if(ret[i,"change"]!="no change no override"){#5
            ret[i,"change"] = "positive"
          }#5
        }#4
        else{#6
          if(ds[i,columns[j]] == "Auf keinen Fall nutzen"){#7
            ret[i,"change"] = "no change no override"
          }#7  
        }#6
      }#3
      
      #  negative outset
      if(ds$app_install[i] == "Wahrscheinlich nicht nutzen"){#3
        if(ds[i,columns[j]] == "Auf jeden Fall nutzen" | ds[i,columns[j]] == "Eher nutzen"){#4
          if(ret[i,"change"]!="no change no override"){#5
            ret[i,"change"] = "positive"
          }#5
        }#4
        else{#6
          if(ds[i,columns[j]] == "Auf keinen Fall nutzen" | ds[i,columns[j]] == "Eher nicht nutzen"){#7
            ret[i,"change"] = "no change no override"
          }#7  
        }#6
      }#3
      
      
      
      #neutral outset
      if(ds$app_install[i] == "Ich bin unentschieden"){#3 #todo Weiß ich nicht hinzufügen. Das ß macht encoding probleme
        if(ds[i,columns[j]] == "Auf jeden Fall nutzen" | ds[i,columns[j]] == "Eher nutzen"){#4
          if(ret[i,"change"]!="negative no override"){#5
            ret[i,"change"] = "positive"
          }#5
        }#4
        else{#6
          if(ds[i,columns[j]] == "Auf keinen Fall nutzen" | ds[i,columns[j]] == "Eher nicht nutzen"){#7
            ret[i,"change"] = "negative no override"
          }#7  
        }#6
      }#3
      
      
    }#for j
  }#for i
  
  for(i in 1:length(q$app_install)) {
    if(ret[i,"change"] == "negative no override"){
      ret[i,"change"] = "negative"
    }
    if(ret[i,"change"] == "no change no override"){
      ret[i,"change"] = "no change"
    }
  }
  
  
  return(ret)
}




patternFinder = function(ds, columns, nr){
  print("Finding patterns:")
  #ret=data.frame(ds$app_install)
  ret=ds
  ret[,"WeißNichtCount"] = 0
  ret[,"AufJedenFallCount"] = 0
  ret[,"EherCount"] = 0
  ret[,"KeinEinflussCount"] = 0
  ret[,"AufKeinenFallCount"] = 0
  ret[,"EherNichtCount"] = 0
  ret[,"YesCount"] = 0
  ret[,"NoCount"] = 0
  
  
  
  for(i in 1:length(q$app_install)) {
    for(j in 1:length(columns)) {
      
      if(ds[i,columns[j]] == "Weiß ich nicht"){#2
        ret[i,"WeißNichtCount"] = ret[i,"WeißNichtCount"]+1
      }
      if(ds[i,columns[j]] == "Auf keinen Fall nutzen"){#2
        ret[i,"AufKeinenFallCount"] = ret[i,"AufKeinenFallCount"]+1
        ret[i,"NoCount"] = ret[i,"NoCount"]+1
      }
      if(ds[i,columns[j]] == "Eher nicht nutzen"){#2
        ret[i,"EherNichtCount"] = ret[i,"EherNichtCount"]+1
        ret[i,"NoCount"] = ret[i,"NoCount"]+1
      }
      if(ds[i,columns[j]] == "Kein Einfluss"){#2
        ret[i,"KeinEinflussCount"] = ret[i,"KeinEinflussCount"]+1
      }
      if(ds[i,columns[j]] == "Eher nutzen"){#2
        ret[i,"EherCount"] = ret[i,"EherCount"]+1
        ret[i,"YesCount"] = ret[i,"YesCount"]+1
      }
      if(ds[i,columns[j]] == "Auf jeden Fall nutzen"){#2
        ret[i,"AufJedenFallCount"] = ret[i,"AufJedenFallCount"]+1
        ret[i,"YesCount"] = ret[i,"YesCount"]+1
      }
    }
    
    ret[i,"Pattern_ExtremeYes"] = 0
    ret[i,"Pattern_ExtremeNo"] = 0
    ret[i,"Pattern_Line"] = 0
    ret[i,"Pattern_Contradiction"] = 0
    ret[i,"Pattern_Line_WeißNicht"] = 0
    ret[i,"Pattern_Line_KeinEinfluss"] = 0
    
   
    
    if(ret[i,"WeißNichtCount"]>=nr){
      ret[i,"Pattern_Line"]=1
      ret[i,"Pattern_Line_WeißNicht"] = 1
      }
    if(ret[i,"AufJedenFallCount"]>=nr){
      ret[i,"Pattern_Line"]=1
      ret[i,"Pattern_Contradiction"] = 1# this will be overriden later if it is not a contradiciton
      }
    if(ret[i,"EherCount"]>=nr){
      ret[i,"Pattern_Line"]=1
      ret[i,"Pattern_Contradiction"] = 1# this will be overriden later if it is not a contradiciton
      }
    if(ret[i,"KeinEinflussCount"]>=nr){
      ret[i,"Pattern_Line"]=1
      ret[i,"Pattern_Line_KeinEinfluss"] = 1
      }
    if(ret[i,"AufKeinenFallCount"]>=nr){
      ret[i,"Pattern_Line"]=1
      ret[i,"Pattern_Contradiction"] = 1# this will be overriden later if it is not a contradiciton
      }
    if(ret[i,"EherNichtCount"]>=nr){
      ret[i,"Pattern_Line"]=1
      ret[i,"Pattern_Contradiction"] = 1 # this will be overriden later if it is not a contradiciton
      }
    
    
    
    if(ds$app_install[i] == "Wahrscheinlich nicht nutzen"|ds$app_install[i] == "Auf keinen Fall nutzen"){
      if(ret[i,"NoCount"]>=nr){
        ret[i,"Pattern_ExtremeNo"] = 1
        ret[i,"Pattern_Contradiction"] = 0
        }
    }
    
    if(ds$app_install[i] == "Wahrscheinlich nutzen"|ds$app_install[i] == "Auf jeden Fall nutzen"){
      if(ret[i,"YesCount"]>=nr){
        ret[i,"Pattern_ExtremeYes"] = 1
        ret[i,"Pattern_Contradiction"] = 0
        }
    }
    
  }
  return(ret)
}


allFactorsColums = c("app_factors_1","app_factors_2","app_factors_3","app_factors_4","app_factors_5","app_factors_6","app_factors_7","app_factors_8","app_factors_9","app_factors_10","app_factors_11","app_factors_12","app_factors_13","app_factors_14","app_factors_15","app_factors_16","app_factors_17","app_factors_18","app_factors_19","app_factors_20","app_factors_21","app_factors_22","app_factors_23","app_factors_24")
allOrderColumns = c("app_factors_DO_1","app_factors_DO_2","app_factors_DO_3","app_factors_DO_4","app_factors_DO_5","app_factors_DO_6","app_factors_DO_7","app_factors_DO_8","app_factors_DO_9","app_factors_DO_10","app_factors_DO_11","app_factors_DO_12","app_factors_DO_13","app_factors_DO_14","app_factors_DO_15","app_factors_DO_16","app_factors_DO_17","app_factors_DO_18","app_factors_DO_19","app_factors_DO_20","app_factors_DO_21","app_factors_DO_22","app_factors_DO_23","app_factors_DO_24")
#allOrderColumns = c("app_factors_DO_1","app_factors_DO_2","app_factors_DO_3","app_factors_DO_4","app_factors_DO_5","app_factors_DO_6","app_factors_DO_7","app_factors_DO_8","app_factors_DO_9","app_factors_DO_10","app_factors_DO_11","app_factors_DO_12","app_factors_DO_13","app_factors_DO_14","app_factors_DO_15","app_factors_DO_16","app_factors_DO_17","app_factors_DO_18","app_factors_DO_19")

#tmp = orderRestorer(q,allFactorsColums,allOrderColumns)

zigZagDetector = function(ds){ #detect zig zag every other row
  ret = data.frame(ds[,1])
  ret[,"zigzag"] = TRUE # assume zig zag 
  
  
  for(i in 1:nrow(ds)){
    for(j in 1:(ncol(ds)-2)){
      if(ds[i,j]!=ds[i,j+2]){
        ret[i,"zigzag"]=FALSE # if second row not the same then no zig zag
      }
    }
  
  }
  
  for(i in 1:nrow(ds)){
      if(ds[i,1]==ds[i,2]){
        ret[i,"zigzag"]=FALSE # if line then also no zig zag
      }
  }
  
  return(ret$zigzag)
}

orderRestorer = function(ds, columns, orderColumns){
  print("Finding patterns:")
  
  ret=data.frame(ds$app_install)
  
  
  for(j in 1:length(columns)) {
    
    ret[,paste("Ordered",columns[j],sep="_")] = NA
    
  }
  
  
  for(i in 1:length(q$app_install)) {
    
    
    for(j in 1:length(columns)) {
      
      
        #ret[i,paste("Ordered",columns[j],sep="_")] = paste(i,j)#ds[i,allFactorsColums[ds[i,allOrderColumns[j]]]]
      #s = ds[i,allFactorsColums[ds[i,allOrderColumns[j]]]]
      #if(i<2&j<10) {
      #  get the name of colum which i saw at jth place e.g j=1 then app_factors_DO_1
      #  print(paste("allOrderColumns[j]",allOrderColumns[j]))
      
      #  get the number of the factor which i saw at jth place, eg 14
      #  print(paste("ds[i,allOrderColumns[j]]",ds[i,allOrderColumns[j]]))
      
      #  get the name of the factors column that i saw at jth place e.g. app_factors_14
      #  get the answer to that factor question
      #  print(paste("allFactorsColums[ds[i,allOrderColumns[j]]]",allFactorsColums[ds[i,allOrderColumns[j]]]))
      #  print(s)
      #  }
      ret[i,paste("Ordered",columns[j],sep="_")] = ds[i,allFactorsColums[ds[i,allOrderColumns[j]]]]
      
      
    }
  }
  #return(ret[,2:ncol(ret)])
  return(ret[,2:20]) # return all columns up to the attention check question and remove dummy first column
}

q[1,allFactorsColums[q[1,allOrderColumns[1]]]]

#sort levels

q$app_install=factor(q$app_install,levels=c("Auf keinen Fall nutzen","Wahrscheinlich nicht nutzen","Ich bin unentschieden","Weiß ich nicht","Möchte ich nicht sagen","Wahrscheinlich nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"app_install2"]=factor(q$app_install,levels=c("Auf keinen Fall nutzen","Wahrscheinlich nicht nutzen","Ich bin unentschieden","Wahrscheinlich nutzen","Auf jeden Fall nutzen"),ordered = TRUE)


q$income = factor(q$income,levels=c("bis 1300","1300-1700€", "1700-2600€", "2600-3600€", "3600-5000€", "über 5000€","Möchte ich nicht sagen"),ordered = TRUE)

q[,"household"]=factor(q$Q98,levels=c("1","2","3","4 oder mehr","Möchte ich nicht sagen"),ordered = TRUE)

#qn=qn[!is.na(q$app_install),]
#qn=qn[!is.na(q$app_install),]
#q=q[!is.na(q$app_install),]
q$age=as.numeric(as.character(q$age))



#media sources
q[,"MS_FreundInnen"]=as.numeric(qn$media_sources_1)
q[,"MS_Familie"]=as.numeric(qn$media_sources_2)
q[,"MS_KollegInnen"]=as.numeric(qn$media_sources_3)
q[,"MS_ÖffentlichRechtlicheSender"]=as.numeric(qn$media_sources_4)
q[,"MS_Private_Sender"]=as.numeric(qn$media_sources_5)
q[,"MS_Zeitungen"]=as.numeric(qn$media_sources_6)
q[,"MS_SozialeMedien"]=as.numeric(qn$media_sources_7)
q[,"MS_StaatlicheWebseiten"]=as.numeric(qn$media_sources_8)
q[,"MS_OffizielleHomepage"]=as.numeric(qn$media_sources_14)
q[,"MS_Webseiten"]=as.numeric(qn$media_sources_9)
q[,"MS_WissenschaftlicheVeröffentlichungen"]=as.numeric(qn$media_sources_10)
q[,"MS_Nichts"]=as.numeric(qn$media_sources_11)
q[,"MS_WeißNicht"]=as.numeric(qn$media_sources_13)


q$MS_FreundInnen=replace(q$MS_FreundInnen,is.na(q$MS_FreundInnen),0) #replace NAs with 0
q$MS_Familie=replace(q$MS_Familie,is.na(q$MS_Familie),0) #replace NAs with 0
q$MS_KollegInnen=replace(q$MS_KollegInnen,is.na(q$MS_KollegInnen),0) #replace NAs with 0
q$MS_ÖffentlichRechtlicheSender=replace(q$MS_ÖffentlichRechtlicheSender,is.na(q$MS_ÖffentlichRechtlicheSender),0) #replace NAs with 0
q$MS_Private_Sender=replace(q$MS_Private_Sender,is.na(q$MS_Private_Sender),0) #replace NAs with 0
q$MS_Zeitungen=replace(q$MS_Zeitungen,is.na(q$MS_Zeitungen),0) #replace NAs with 0
q$MS_SozialeMedien=replace(q$MS_SozialeMedien,is.na(q$MS_SozialeMedien),0) #replace NAs with 0
q$MS_StaatlicheWebseiten=replace(q$MS_StaatlicheWebseiten,is.na(q$MS_StaatlicheWebseiten),0) #replace NAs with 0
q$MS_OffizielleHomepage=replace(q$MS_OffizielleHomepage,is.na(q$MS_OffizielleHomepage),0) #replace NAs with 0
q$MS_Webseiten=replace(q$MS_Webseiten,is.na(q$MS_Webseiten),0) #replace NAs with 0
q$MS_WissenschaftlicheVeröffentlichungen=replace(q$MS_WissenschaftlicheVeröffentlichungen,is.na(q$MS_WissenschaftlicheVeröffentlichungen),0) #replace NAs with 0
q$MS_Nichts=replace(q$MS_Nichts,is.na(q$MS_Nichts),0) #replace NAs with 0
q$MS_WeißNicht=replace(q$MS_WeißNicht,is.na(q$MS_WeißNicht),0) #replace NAs with 0



media_sources=c(sum(q$MS_FreundInnen,na.rm=T),sum(q$MS_Familie,na.rm=T),sum(q$MS_KollegInnen,na.rm=T))
#todo

q[,"A_Nähe"]=as.numeric(qn$app_attributes_gen_2) 
q[,"A_Verpflichtend"]=as.numeric(qn$app_attributes_gen_4)
q[,"A_Bluetooth"]=as.numeric(qn$app_attributes_gen_5)
q[,"A_GPS"]=as.numeric(qn$app_attributes_gen_6)
q[,"A_Spenden"]=as.numeric(qn$app_attributes_gen_8)
q[,"A_Zeigen"]=as.numeric(qn$app_attributes_gen_10)
q[,"A_Befinden"]=as.numeric(qn$app_attributes_gen_11)
q[,"A_Privatsphäre"]=as.numeric(qn$app_attributes_gen_12)
q[,"A_Grundrechte"]=as.numeric(qn$app_attributes_gen_13)
q[,"A_Bekämpfen"]=as.numeric(qn$app_attributes_gen_14)
q[,"A_Telefonnummer"]=as.numeric(qn$app_attributes_gen_15)
q[,"A_Sicherheitsabstand"]=as.numeric(qn$app_attributes_gen_16)
q[,"A_Temp"]=as.numeric(qn$app_attributes_gen_18)
q[,"A_Fitness"]=as.numeric(qn$app_attributes_gen_24)
q[,"A_Profil"]=as.numeric(qn$app_attributes_gen_26)
q[,"A_Testen"]=as.numeric(qn$app_attributes_gen_23)
q[,"A_Keine"]=as.numeric(qn$app_attributes_gen_21)
q[,"A_WeissNicht"]=as.numeric(qn$app_attributes_gen_22)

q$A_Nähe=replace(q$A_Nähe,is.na(q$A_Nähe),0) #replace NAs with 0
q$A_Verpflichtend=replace(q$A_Verpflichtend,is.na(q$A_Verpflichtend),0)
q$A_Bluetooth=replace(q$A_Bluetooth,is.na(q$A_Bluetooth),0)
q$A_GPS=replace(q$A_GPS,is.na(q$A_GPS),0)
q$A_Spenden=replace(q$A_Spenden,is.na(q$A_Spenden),0)
q$A_Zeigen=replace(q$A_Zeigen,is.na(q$A_Zeigen),0)
q$A_Befinden=replace(q$A_Befinden,is.na(q$A_Befinden),0)
q$A_Privatsphäre=replace(q$A_Privatsphäre,is.na(q$A_Privatsphäre),0)
q$A_Grundrechte=replace(q$A_Grundrechte,is.na(q$A_Grundrechte),0)
q$A_Bekämpfen=replace(q$A_Bekämpfen,is.na(q$A_Bekämpfen),0)
q$A_Telefonnummer=replace(q$A_Telefonnummer,is.na(q$A_Telefonnummer),0)
q$A_Sicherheitsabstand=replace(q$A_Sicherheitsabstand,is.na(q$A_Sicherheitsabstand),0)
q$A_Temp=replace(q$A_Temp,is.na(q$A_Temp),0)
q$A_Fitness=replace(q$A_Fitness,is.na(q$A_Fitness),0)
q$A_Profil=replace(q$A_Profil,is.na(q$A_Profil),0)
q$A_Testen=replace(q$A_Testen,is.na(q$A_Testen),0)
q$A_Keine=replace(q$A_Keine,is.na(q$A_Keine),0)
q$A_WeissNicht=replace(q$A_WeissNicht,is.na(q$A_WeissNicht),0)


q[,"AO_Nähe"]=as.numeric(qn$app_attributes_other_7)
q[,"AO_Nachhinein"]=as.numeric(qn$app_attributes_other_9)
q[,"AO_Quarantäne"]=as.numeric(qn$app_attributes_other_17)
q[,"AO_Keine"]=as.numeric(qn$app_attributes_other_21)
q[,"AO_WeissNicht"]=as.numeric(qn$app_attributes_other_22)

q$AO_Nähe=replace(q$AO_Nähe,is.na(q$AO_Nähe),0)#replace NAs with 0
q$AO_Nachhinein=replace(q$AO_Nachhinein,is.na(q$AO_Nachhinein),0)
q$AO_Quarantäne=replace(q$AO_Quarantäne,is.na(q$AO_Quarantäne),0)
q$AO_Keine=replace(q$AO_Keine,is.na(q$AO_Keine),0)
q$AO_WeissNicht=replace(q$AO_WeissNicht,is.na(q$AO_WeissNicht),0)



q[,"AS_Nachhinein"]=as.numeric(qn$app_attribues_self_1)
q[,"AS_Arzt"]=as.numeric(qn$app_attribues_self_3)
q[,"AS_Kontinuierlich"]=as.numeric(qn$app_attribues_self_23)
q[,"AS_Quarantäne "]=as.numeric(qn$app_attribues_self_17)
q[,"AS_Keine"]=as.numeric(qn$app_attribues_self_21)
q[,"AS_WeissNicht"]=as.numeric(qn$app_attribues_self_22)

q$AS_Nachhinein=replace(q$AS_Nachhinein,is.na(q$AS_Nachhinein),0)#replace NAs with 0
q$AS_Arzt=replace(q$AS_Arzt,is.na(q$AS_Arzt),0)
q$AS_Kontinuierlich=replace(q$AS_Kontinuierlich,is.na(q$AS_Kontinuierlich),0)
q$AS_Quarantäne=replace(q$AS_Quarantäne,is.na(q$AS_Quarantäne),0)
q$AS_Keine=replace(q$AS_Keine,is.na(q$AS_Keine),0)
q$AS_WeissNicht=replace(q$AS_WeissNicht,is.na(q$AS_WeissNicht),0)




q[,"comp_bluetooth"]=as.numeric(qn$comprehension_check_1)
q[,"comp_WLAN"]=as.numeric(qn$comprehension_check_2)
q[,"comp_GPS"]=as.numeric(qn$comprehension_check_3)
q[,"comp_Telefon"]=as.numeric(qn$comprehension_check_5)
q[,"comp_WeissNicht"]=as.numeric(qn$comprehension_check_4)

q$comp_bluetooth=replace(q$comp_bluetooth,is.na(q$comp_bluetooth),0)#replace NAs with 0
q$comp_WLAN=replace(q$comp_WLAN,is.na(q$comp_WLAN),0)
q$comp_GPS=replace(q$comp_GPS,is.na(q$comp_GPS),0)
q$comp_Telefon=replace(q$comp_Telefon,is.na(q$comp_Telefon),0)
q$comp_WeissNicht=replace(q$comp_WeissNicht,is.na(q$comp_WeissNicht),0)

#for clickworker
#q$comp_bluetooth = 0
#q$comp_bluetooth = q$comprehension_check=="Bluetooth"
#q$comp_bluetooth = as.numeric(q$comp_bluetooth)






q[,"F_Ansteckungsrisiko"]=factor(q$app_factors_1,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Weiß ich nicht","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_Andere"]=factor(q$app_factors_2,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Weiß ich nicht","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_Sonst_Nicht"]=factor(q$app_factors_3,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Weiß ich nicht","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_Einschätzen"]=factor(q$app_factors_4,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Weiß ich nicht","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_Leben"]=factor(q$app_factors_5,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Weiß ich nicht","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_Wirtschaft"]=factor(q$app_factors_6,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Weiß ich nicht","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)

q[,"F_RKI_Sehen_Andere"]=factor(q$app_factors_7,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Weiß ich nicht","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_RKI_Sehen_Ansteckungsrisiko"]=factor(q$app_factors_8,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Weiß ich nicht","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_Hacker"]=factor(q$app_factors_9,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Weiß ich nicht","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_Mindestabstand"]=factor(q$app_factors_10,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Weiß ich nicht","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_Aufenthaltsort"]=factor(q$app_factors_11,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Weiß ich nicht","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_FP_Quarantäne"]=factor(q$app_factors_12,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Weiß ich nicht","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)

q[,"F_FP_Testen"]=factor(q$app_factors_13,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Weiß ich nicht","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_Quarantäne_Verordnet"]=factor(q$app_factors_14,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Weiß ich nicht","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_Datenbank_Infizierten"]=factor(q$app_factors_15,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Weiß ich nicht","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_Gesetz"]=factor(q$app_factors_16,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Weiß ich nicht","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_DGSVO"]=factor(q$app_factors_17,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Weiß ich nicht","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)

q[,"F_Technisch"]=factor(q$app_factors_18,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Weiß ich nicht","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_BSI"]=factor(q$app_factors_19,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Weiß ich nicht","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_Experten"]=factor(q$app_factors_20,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Weiß ich nicht","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_Keinen_Aufenthaltsort"]=factor(q$app_factors_21,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Weiß ich nicht","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_Attention_Check"]=factor(q$app_factors_22,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Weiß ich nicht","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_Open_Source"]=factor(q$app_factors_23,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Weiß ich nicht","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_Gesetz_Nicht_Technisch"]=factor(q$app_factors_24,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Weiß ich nicht","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)

#drop don't want to says
q[,"worry_health"]=factor(q$worry_1,levels=c("Unbesorgt","Ein wenig besorgt","Besorgt","Sehr besorgt"),ordered = TRUE)
q[,"worry_economy"]=factor(q$worry_2,levels=c("Unbesorgt","Ein wenig besorgt","Besorgt","Sehr besorgt"),ordered = TRUE)
q[,"worry_social"]=factor(q$worry_3,levels=c("Unbesorgt","Ein wenig besorgt","Besorgt","Sehr besorgt"),ordered = TRUE)


###################remove Weiß Nicht and story in xxx2
q[,"F_Ansteckungsrisiko2"]=factor(q$app_factors_1,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_Andere2"]=factor(q$app_factors_2,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_Sonst_Nicht2"]=factor(q$app_factors_3,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_Einschätzen2"]=factor(q$app_factors_4,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_Leben2"]=factor(q$app_factors_5,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_Wirtschaft2"]=factor(q$app_factors_6,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)

q[,"F_RKI_Sehen_Andere2"]=factor(q$app_factors_7,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_RKI_Sehen_Ansteckungsrisiko2"]=factor(q$app_factors_8,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_Hacker2"]=factor(q$app_factors_9,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_Mindestabstand2"]=factor(q$app_factors_10,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_Aufenthaltsort2"]=factor(q$app_factors_11,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_FP_Quarantäne2"]=factor(q$app_factors_12,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)

q[,"F_FP_Testen2"]=factor(q$app_factors_13,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_Quarantäne_Verordnet2"]=factor(q$app_factors_14,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_Datenbank_Infizierten2"]=factor(q$app_factors_15,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_Gesetz2"]=factor(q$app_factors_16,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_DGSVO2"]=factor(q$app_factors_17,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)

q[,"F_Technisch2"]=factor(q$app_factors_18,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_BSI2"]=factor(q$app_factors_19,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_Experten2"]=factor(q$app_factors_20,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_Keinen_Aufenthaltsort2"]=factor(q$app_factors_21,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_Attention_Check2"]=factor(q$app_factors_22,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_Open_Source2"]=factor(q$app_factors_23,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)
q[,"F_Gesetz_Nicht_Technisch2"]=factor(q$app_factors_24,levels=c("Auf keinen Fall nutzen","Eher nicht nutzen","Kein Einfluss","Eher nutzen","Auf jeden Fall nutzen"),ordered = TRUE)

#####################
allFactors = c("F_Ansteckungsrisiko","F_Andere","F_Sonst_Nicht","F_Einschätzen","F_Leben","F_Wirtschaft","F_RKI_Sehen_Andere","F_RKI_Sehen_Ansteckungsrisiko","F_Hacker","F_Mindestabstand","F_Aufenthaltsort"
   ,"F_FP_Quarantäne","F_FP_Testen","F_Quarantäne_Verordnet","F_Datenbank_Infizierten","F_Gesetz","F_DGSVO","F_Technisch","F_BSI","F_Experten","F_Keinen_Aufenthaltsort","F_Open_Source","F_Gesetz_Nicht_Technisch")
allQuestions = c("Die App würde mich benachrichtigen, falls ich einem COVID-19 Ansteckungsrisiko ausgesetzt war. ","Falls ich positiv auf COVID-19 getestet werde, würde die App automatisch andere NutzerInnen der App benachrichtigen, die sich bei mir anstecken konnten. ","Die App würde Menschen über Ansteckungsrisiken informieren, die darüber sonst nicht vom Gesundheitsamt informiert werden würden. ","Die App würde dem RKI helfen die COVID-19 Lage in Deutschland besser einzuschätzen. ","Die Nutzung der App würde es der Bevölkerung ermöglichen schneller zum öffentlichen Leben zurückzukehren. ","Die Nutzung der App würde es ermöglichen, die Wirtschaft schneller wieder hochzufahren. ","Falls ich positiv auf COVID-19 getestet werde, würde die App es dem RKI ermöglichen zu sehen mit wem ich Kontakt hatte, um diese Personen zu informieren. ","Falls jemand in meinem Umfeld positiv auf COVID-19 getestet wird, würde die App es dem RKI ermöglichen zu sehen, dass ich mit dieser Person Kontakt hatte, um mich zu informieren. ","Falls ich positiv auf COVID-19 getestet werde, könnten Hacker in meiner Umgebung rausfinden, dass ich infiziert bin. ","Die Nutzung der App würde es dem RKI ermöglichen zu erfahren, wenn ich den Mindestabstand zu anderen Personen nicht einhalte. ","Die App würde Informationen über meinen Aufenthaltsort nutzen, um das Infektionsrisiko für andere besser einzuschätzen. ","Es bestünde die Möglichkeit, dass die App fälschlicherweise ein Ansteckungsrisiko meldet und ich mich deshalb unnötig in Quarantäne begeben muss. ","Es bestünde die Möglichkeit, dass die App fälschlicherweise ein Ansteckungsrisiko meldet und ich mich deshalb unnötig testen lasse. ","Falls mich die App über ein Ansteckungsrisiko informiert, müsste ich mich in gesetzlich verordnete Quarantäne begeben. ","Das RKI hätte eine Datenbank mit Kontaktinformationen von Infizierten und von Personen mit denen diese Kontakt hatten. ","Der Schutz der Daten würde durch ein  für die App entworfenes, neues Gesetz gewährleistet. ","Der Schutz der Daten würde durch eine Datenschutzerklärung und die Datenschutzgrundverordnung gewährleistet. ","Der Schutz der Daten würde durch technische Maßnahmen gewährleistet. ","Die App würde vom deutschen Bundesamt für Sicherheit in der Informationstechnik (BSI) auf Datensicherheit und Datenschutz überprüft. ","Die App würde von unabhängigen SicherheitsexpertInnen auf Datensicherheit und Datenschutz überprüft. ","Die App würde keine Informationen über meinen Aufenthaltsort erheben.","Die App wäre Open Source.","Der Staat würde durch Gesetze, aber nicht durch technische Maßnahmen daran gehindert, die Daten für Überwachung zu missbrauchen.")
factorQuestionMapping=data.frame(allQuestions,allFactors)

q[,"entrypass"]=factor(q$entrypass,levels=c("Voll ablehnen","Eher ablehnen","Weder befürworten noch ablehnen","Weiß ich nicht","Möchte ich nicht sagen","Eher befürworten","Voll befürworten"),ordered = TRUE)
q[,"entrypass2"]=factor(q$entrypass,levels=c("Voll ablehnen","Eher ablehnen","Weder befürworten noch ablehnen","Eher befürworten","Voll befürworten"),ordered = TRUE)



q$party_preference=as.factor((q$party_preference))


#table(qNo$app_install)
#table(qYes$app_install)
#table(qUn$app_install)


#check inconsistencies

orderedFactors = orderRestorer(q,allFactorsColums,allOrderColumns)
zigzag = zigZagDetector(orderedFactors)

print(paste("Nr. of zig zaggers:",sum(as.numeric(zigzag))))

#patterns=patternFinder(q,allFactors,22)
q=patternFinder(q,allFactors,22)

q[,"Knock_out_Pattern"]=q$Pattern_Contradiction
q[,"Saved_by_Pattern"]=q$Pattern_Line

  
#These two questions are based on the same fact and thus should mostly be answered the same way
q[,"Warn_Quarantäne"]=q$AO_Quarantäne+q$AS_Quarantäne # 0 on both or 1 on both are fine. 0 on one and 1 on the other is strange
q$Warn_Quarantäne[q$Warn_Quarantäne==2]=0 # so replace 2 with 0 leaving 1 as the warning
sum(q$Warn_Quarantäne)

#These two questions are based on the same fact and thus should mostly be answered the same way
q[,"Warn_Nachhinein"]=q$AO_Nachhinein+q$AS_Nachhinein
q$Warn_Nachhinein[q$Warn_Nachhinein==2]=0
sum(q$Warn_Nachhinein)

#soft: bluetooth question was not clicked (0). Assumption: Bluetooth should have been marked.
q[,"Warn_Comp_Bluetooth"]=q$comp_bluetooth
q$Warn_Comp_Bluetooth[q$Warn_Comp_Bluetooth==1]=99 # 99 is a placeholder to switch 0 and 1s
q$Warn_Comp_Bluetooth[q$Warn_Comp_Bluetooth==0]=1
q$Warn_Comp_Bluetooth[q$Warn_Comp_Bluetooth==99]=0
sum(q$Warn_Comp_Bluetooth)

#soft: bluetooth combined with tech-question: Yes # double penalty for tech people getting bluetooth wrong
#comp_bluetooth =0 0 haben and tech == 1 (yes)
q[,"Warn_Comp_Bluetooth_Tech"]=q$comp_bluetooth
q$Warn_Comp_Bluetooth_Tech[q$comp_bluetooth==0 & q$tech == "Ja"] = 99
q$Warn_Comp_Bluetooth_Tech[q$Warn_Comp_Bluetooth_Tech!=99] = 0
q$Warn_Comp_Bluetooth_Tech[q$Warn_Comp_Bluetooth_Tech==99] = 1
sum(q$Warn_Comp_Bluetooth_Tech)



#technical: factors site is answered to fast
#boxplot.stats(q$time_factors_Page.Submit)
#sort(q$time_factors_Page.Submit)
# -> wir haben wirklich einen fliesenden Übergang von 13 Skeunden bis 470 ... max(6553), wenig eüber 1000. 200 unter 100
#Intuitiv ~120 Sekunden ist schnell 285 sind drunter
#An den Einträgen sind die nicht eindeutig/mehrheit,ich als quatsch zu idenitifzieren: q$app_install_reason[q$time_factors_Page.Submit < 120]

#soft: app description speeded
#q$time_app_description_Page.Submit
#-> ~100 unter 10 Sekunden. sortq($time_app_description_Page.Submit), max(3558.47)
#Die sind, gemessen an der Installationsfrage auch nicht einfach auszusortieren.


#soft: app description is not read with attention (to fast)
#soft: multiple choice seems off. "Ich weiß es nicht" + andere
#media_sources
q[,"Warn_MultipleChoice_Media_IDK"] = rowSums(qn[,c("media_sources_11", "media_sources_13")], na.rm=TRUE)
q[,"Warn_MultipleChoice_Media_IK"] = rowSums(qn[,c("media_sources_1", "media_sources_2", "media_sources_10", "media_sources_9", "media_sources_14", "media_sources_8", "media_sources_7", "media_sources_6", "media_sources_5", "media_sources_4", "media_sources_3")], na.rm=TRUE)
q$Warn_MultipleChoice_Media[q$Warn_MultipleChoice_Media_IDK > 0 & q$Warn_MultipleChoice_Media_IK > 0] = 1
q$Warn_MultipleChoice_Media[is.na(q$Warn_MultipleChoice_Media)] = 0
sum(q$Warn_MultipleChoice_Media)

#app_attributes_gen
q[,"Warn_MultipleChoice_gen_IDK"] = q$app_attributes_other_22
q[,"Warn_MultipleChoice_gen_IK"] = rowSums(qn[,c("app_attributes_gen_2", "app_attributes_gen_4", "app_attributes_gen_5", "app_attributes_gen_6", "app_attributes_gen_8", "app_attributes_gen_10", "app_attributes_gen_11", "app_attributes_gen_12", "app_attributes_gen_13", "app_attributes_gen_14", "app_attributes_gen_15", "app_attributes_gen_16", "app_attributes_gen_18", "app_attributes_gen_24", "app_attributes_gen_26", "app_attributes_gen_23", "app_attributes_gen_21", "app_attributes_gen_22", "app_attributes_other_7", "app_attributes_other_9", "app_attributes_other_17", "app_attributes_other_21")], na.rm=TRUE)
q$Warn_MultipleChoice_Gen[q$Warn_MultipleChoice_gen_IDK == "Weiß ich nicht" & q$Warn_MultipleChoice_gen_IK > 0] = 1
q$Warn_MultipleChoice_Gen[is.na(q$Warn_MultipleChoice_Gen)] = 0
sum(q$Warn_MultipleChoice_Gen)

#app_attributes_other
#app_attribues_self
#comprehension_check


q[,"Knock_out_Attention_Check"]=as.character(q$F_Attention_Check)
q$Knock_out_Attention_Check[q$Knock_out_Attention_Check!="Kein Einfluss"]=1
q$Knock_out_Attention_Check[q$Knock_out_Attention_Check=="Kein Einfluss"]=0
q$Knock_out_Attention_Check=replace(q$Knock_out_Attention_Check,is.na(q$Knock_out_Attention_Check),1)#replace NAs with 1
q$Knock_out_Attention_Check=as.numeric(q$Knock_out_Attention_Check)
sum(q$Knock_out_Attention_Check) # since qulatrics removed these already this is redundant 



# participants who gave a positive answer to the hacker question but were also positive about the app before 
q[,"IDoNotMindHackers"]=as.numeric((q$F_Hacker == "Auf jeden Fall nutzen"| q$F_Hacker == "Eher nutzen") & (q$app_install=="Auf jeden Fall nutzen" | q$app_install=="Wahrscheinlich nutzen" ))
sum(q$IDoNotMindHackers)

# if someone initially did not want to use the app but after hearing that hackers could access the data they want to use the app
q[,"Warn_ILoveHackers"]=as.numeric((q$F_Hacker == "Auf jeden Fall nutzen" | q$F_Hacker == "Eher nutzen") & (q$app_install=="Auf keinen Fall nutzen" | q$app_install=="Wahrscheinlich nicht nutzen" | q$app_install=="Ich bin unentschieden"| q$app_install=="Weiß ich nicht"))
sum(q$Warn_ILoveHackers)


# if a participant was positive about the app but turns negative because it could help the economy 
q[,"Warn_IHateTheEconomy1"]=as.numeric(noVec(q$F_Wirtschaft) & installYesUnVec(q))
sum(q$Warn_IHateTheEconomy1)

# - Menschen informieren, die sonst nicht informiert werden würden, YesWay + "Auf keinen Fall"
#q[q$app_factors_3 == "Auf keinen Fall nutzen" & q$app_install == "Auf jeden Fall nutzen","WarnenStrange"] = 1
#q$WarnenStrange[is.na(q$WarnenStrange)] = 0
q[,"Warn_IHateOthers"]=as.numeric(noVec(q$F_Sonst_Nicht) & installYesUnVec(q))
sum(q$Warn_IHateOthers)


# - false positives Quarantäne  "Auf jeden Fall" + NoWay 
#q[q$app_factors_12 == "Auf jeden Fall nutzen" & q$app_install == "Auf keinen Fall nutzen","QuarantStrange"] = 1
#q$QuarantStrange[is.na(q$QuarantStrange)] = 0
q[,"Warn_ILoveWrongQuarntine"]=as.numeric(yesVec(q$F_FP_Quarantäne) & installNoUnVec(q))
sum(q$Warn_ILoveWrongQuarntine)

#Removed because potentially these are people who don't believe the app works but would use it if the RKI gets good data...
# - positiver Test, RKI sieht den Kontakt, NoWay + "Auf jeden Fall"
#q[q$app_factors_7 == "Auf jeden Fall nutzen" & q$app_install == "Auf keinen Fall nutzen","RKIStrange"] = 1
#q$RKIStrange[is.na(q$RKIStrange)] = 0
#q[,"Warn_ILoveSurveilance"]=as.numeric(yesVec(q$F_RKI_Sehen_Andere) & installNoUnVec(q))


# - „Die Nutzung der App wÃ¼rde es ermÃ¶glichen, die Wirtschaft schneller wieder hochzufahren.“ 
# -> Negativ 
# + Sehr besorgt wegen Wirtschaft
q[q$app_factors_6 == "Auf keinen Fall nutzen" & q$worry_2 == "Sehr besorgt","Warn_IHateTheEconomy2"] = 1
q$Warn_IHateTheEconomy2[is.na(q$Warn_IHateTheEconomy2)] = 0
sum(q$Warn_IHateTheEconomy2)


#removed because this could be someone who is worried about health but is more worried about the app
# - Falls jemand in meinem Umfeld positiv auf COVID-19 getestet wird, wÃ¼rde die App es dem RKI ermÃ¶glichen zu sehen, 
# dass ich mit dieser Person Kontakt hatte, um mich zu informieren. -> negativ
# + Sehr besorgt wegen Gesundheit
#q[q$app_factors_8 == "Auf keinen Fall nutzen" & q$worry_1 == "Sehr besorgt","GesundheitStrange"] = 1
#q[q$app_factors_1 == "Auf keinen Fall nutzen" & q$worry_1 == "Sehr besorgt","GesundheitStrange2"] = 1

#q$GesundheitStrange[is.na(q$GesundheitStrange)] = 0
#q$GesundheitStrange2[is.na(q$GesundheitStrange2)] = 0



# - Wirtschaftliche Existenz ist bedroht + Unbesorgt wegen Wirtschaft
q[q$finances == "Wirtschaftliche Existenz ist bedroht" & q$worry_2=="Unbesorgt","Warn_WirtschaftsExistenz"] = 1
q$Warn_WirtschaftsExistenz[is.na(q$Warn_WirtschaftsExistenz)] = 0
sum(q$Warn_WirtschaftsExistenz)

# entrypass "Voll befürworten" + NoWay  
q[q$app_install == "Auf keinen Fall nutzen" & q$entrypass == "Voll befürworten","Warn_Entry"] = 1
q$Warn_Entry[is.na(q$Warn_Entry)] = 0
sum(q$Warn_Entry)


# beamter + job verloren
q[q$home_office == "Job verloren" & q$job == "Beamter","BeamterLost"] = 1
q$BeamterLost[is.na(q$BeamterLost)] = 0
sum(q$BeamterLost)

#removed because we already discussed that the Linke supporters might still trust the government and we only have 2 AFD people and it is possible that people selected AFD for fun.
# trust_in_government "Stimme vollständig zu" + party_preference (AfD, Die Linke)
#q[,"PartyStrange"]=0
#q[q$trust_in_government == "Stimme vollständig zu" & q$party_preference == "AfD","PartyStrange"] = 1
#q[q$trust_in_government == "Stimme vollständig zu" & q$party_preference == "Die Linke","PartyStrange"] = 1
#q$PartyStrange[is.na(q$PartyStrange)] = 0

# job - Schüler und ein markierter Abschluss bei Q91 (Hochschule)
q[q$job == "SchülerIn" & (q$Q91 == "Fachhochschulabschluss (auch Abschluss einer Ingenieurschule)"|q$Q91 == "Abgeschlossene Lehre"| q$Q91 == "Hochschulabschluss"|q$Q91 == "Berufsfachschulabschluss / Fachschulabschluss"),"Warn_Schüler_Job"] = 1
q$Warn_Schüler_Job[is.na(q$Warn_Schüler_Job)] = 0
sum(q$Warn_Schüler_Job)

#removed I think this is possible. e.g. trump thinks he is invincible 
# worry - Gesundheit "unbesorgt" + risk_own/risk_close/infected/deaths: Yes
#q[q$worry_1 == "Unbesorgt" & q$risk_own == "Ja","GesundheitStrange5"] = 1
#q[q$worry_1 == "Unbesorgt" & q$risk_close == "Ja","GesundheitStrange5"] = 1
#q[q$worry_1 == "Unbesorgt" & q$infected == "Ja","GesundheitStrange5"] = 1
#q[q$worry_1 == "Unbesorgt" & q$deaths == "Ja","GesundheitStrange5"] = 1
#q$GesundheitStrange5[is.na(q$GesundheitStrange5)] = 0


# mental model bluetooth, comprehension nicht bluetooth
q[q$app_attributes_gen_5 == "Die App benutzt Bluetooth." & q$comp_bluetooth == 0,"Warn_Bluetooth_Attr_Comp"] = 1
q$Warn_Bluetooth_Attr_Comp[is.na(q$Warn_Bluetooth_Attr_Comp)] = 0
sum(q$Warn_Bluetooth_Attr_Comp)

# sum: 





sum(q$Warn_Quarantäne)
sum(q$Warn_Nachhinein)
sum(q$Warn_Comp_Bluetooth)
sum(q$Warn_Comp_Bluetooth_Tech)
sum(q$Warn_MultipleChoice_Media)
sum(q$Warn_MultipleChoice_Gen)
sum(q$Warn_ILoveHackers)
sum(q$Warn_IHateTheEconomy1)
sum(q$Warn_IHateOthers)
sum(q$Warn_ILoveWrongQuarntine)
sum(q$Warn_IHateTheEconomy2)
sum(q$Warn_WirtschaftsExistenz)
sum(q$Warn_Entry)
sum(q$BeamterLost)
sum(q$Warn_Schüler_Job)
sum(q$Warn_Bluetooth_Attr_Comp)

q[,"Warn_Count"] = q$Warn_Quarantäne+q$Warn_Nachhinein+q$Warn_Comp_Bluetooth+q$Warn_Comp_Bluetooth_Tech+q$Warn_MultipleChoice_Media+q$Warn_MultipleChoice_Gen+q$Warn_ILoveHackers+q$Warn_IHateTheEconomy1+q$Warn_IHateOthers+q$Warn_ILoveWrongQuarntine+q$Warn_IHateTheEconomy2+q$Warn_WirtschaftsExistenz+q$Warn_Entry+q$BeamterLost+q$Warn_Schüler_Job+q$Warn_Bluetooth_Attr_Comp 
table(q$Warn_Count)



qOriginal = q




#remove participants based on inconsitencies 
#remove those with inconsistent patterns ==1
#qs=qs[q$Knock_out_Pattern==0,]
q=q[q$Knock_out_Pattern==0,]


#remove those who failed attention check (not needed for qualtrics but others)
#qs=qs[q$Knock_out_Attention_Check==0,]
q=q[q$Knock_out_Attention_Check==0,]

qExtremeYes = q[q$Pattern_ExtremeYes==1,]
qExtremeNo = q[q$Pattern_ExtremeNo==1,]

sum(qExtremeNo$Warn_Quarantäne)
sum(qExtremeNo$Warn_Nachhinein)
sum(qExtremeNo$Warn_Comp_Bluetooth)
sum(qExtremeNo$Warn_Comp_Bluetooth_Tech)
sum(qExtremeNo$Warn_MultipleChoice_Media)
sum(qExtremeNo$Warn_MultipleChoice_Gen)
sum(qExtremeNo$Warn_ILoveHackers)
sum(qExtremeNo$Warn_IHateTheEconomy1)
sum(qExtremeNo$Warn_IHateOthers)
sum(qExtremeNo$Warn_ILoveWrongQuarntine)
sum(qExtremeNo$Warn_IHateTheEconomy2)
sum(qExtremeNo$Warn_WirtschaftsExistenz)
sum(qExtremeNo$Warn_Entry)
sum(qExtremeNo$BeamterLost)
sum(qExtremeNo$Warn_Schüler_Job)
sum(qExtremeNo$Warn_Bluetooth_Attr_Comp)


sum(qExtremeYes$Warn_Quarantäne)
sum(qExtremeYes$Warn_Nachhinein)
sum(qExtremeYes$Warn_Comp_Bluetooth)
sum(qExtremeYes$Warn_Comp_Bluetooth_Tech)
sum(qExtremeYes$Warn_MultipleChoice_Media)
sum(qExtremeYes$Warn_MultipleChoice_Gen)
sum(qExtremeYes$Warn_ILoveHackers)
sum(qExtremeYes$Warn_IHateTheEcoYesmy1)
sum(qExtremeYes$Warn_IHateOthers)
sum(qExtremeYes$Warn_ILoveWrongQuarntine)
sum(qExtremeYes$Warn_IHateTheEcoYesmy2)
sum(qExtremeYes$Warn_WirtschaftsExistenz)
sum(qExtremeYes$Warn_Entry)
sum(qExtremeYes$BeamterLost)
sum(qExtremeYes$Warn_Schüler_Job)
sum(qExtremeYes$Warn_Bluetooth_Attr_Comp)

table(qExtremeYes$Warn_Count)
table(qExtremeNo$Warn_Count)

# remove those with 3 or more inconsisten answers
q[,"Warn_Mask"]=q$Warn_Count>=3
#qs=qs[!q$Warn_Mask,]
qn=qn[!q$Warn_Mask,] 
q=q[!q$Warn_Mask,] 

#remove the extreme yes and no because we don't need them for the factor analysis
#qs=qs[q$Pattern_ExtremeNo==0 & q$Pattern_ExtremeYes==0,]
qn=qn[q$Pattern_ExtremeNo==0 & q$Pattern_ExtremeYes==0,]
q= q[q$Pattern_ExtremeNo==0 & q$Pattern_ExtremeYes==0,]



table(q$Warn_Count)
#table(q[q$Saved_by_Pattern==1,]$Warn_Count)
#table(q[q$Saved_by_Pattern==0,]$Warn_Count)




#qIn=q[!q$Warn_Mask,]
#qOut=q[q$Warn_Mask,]

summary(q$time_factors_Page.Submit)

summary(q[q$Saved_by_Pattern==1,]$time_factors_Page.Submit)
summary(q[q$Saved_by_Pattern==0,]$time_factors_Page.Submit)


#summary(qOut$time_factors_Page.Submit)
#summary(qIn$time_factors_Page.Submit)

#tmp = qIn[,c("time_factors_Page.Submit","Warn_Count")]
#tmp = tmp[order(tmp$time_factors_Page.Submit),]
#tmp

#qInTime60 = q[q$time_factors_Page.Submit>60,]
#qInTime60save = q[q$time_factors_Page.Submit>60|q$Saved_by_Pattern==1,]
#qInTime90 = q[q$time_factors_Page.Submit>69,]
#qInTime90save = q[q$time_factors_Page.Submit>=90|q$Saved_by_Pattern==1,]
#qsAll = qs
qAll = q
#qs=qs[q$time_factors_Page.Submit>=90|q$Saved_by_Pattern==1,]
q=q[q$time_factors_Page.Submit>=90|q$Saved_by_Pattern==1,]





#yyy
#check balancing



# split groups

table(q$app_install)

qNo=q[((q$app_install=="Wahrscheinlich nicht nutzen")+(q$app_install=="Auf keinen Fall nutzen"))==1,]
qYes=q[((q$app_install=="Wahrscheinlich nutzen")+(q$app_install=="Auf jeden Fall nutzen"))==1,]
qUn=q[((q$app_install=="Ich bin unentschieden")+(q$app_install=="Weiß ich nicht"))==1,] 


qNoWay=q[(q$app_install=="Auf keinen Fall nutzen")==1,]
qProbNo=q[((q$app_install=="Wahrscheinlich nicht nutzen"))==1,]

qProbYes=q[((q$app_install=="Wahrscheinlich nutzen"))==1,]
qYesWay=q[((q$app_install=="Auf jeden Fall nutzen"))==1,]


####
qAllNo=qAll[((qAll$app_install=="Wahrscheinlich nicht nutzen")+(qAll$app_install=="Auf keinen Fall nutzen"))==1,]
qAllYes=qAll[((qAll$app_install=="Wahrscheinlich nutzen")+(qAll$app_install=="Auf jeden Fall nutzen"))==1,]
qAllUn=qAll[((qAll$app_install=="Ich bin unentschieden")+(qAll$app_install=="Weiß ich nicht"))==1,] #todo do we want the I don't knows in here?


qAllNoWay=qAll[(qAll$app_install=="Auf keinen Fall nutzen")==1,]
qAllProbNo=qAll[((qAll$app_install=="Wahrscheinlich nicht nutzen"))==1,]

qAllProbYes=qAll[((qAll$app_install=="Wahrscheinlich nutzen"))==1,]
qAllYesWay=qAll[((qAll$app_install=="Auf jeden Fall nutzen"))==1,]

####


total=nrow(q) # those you can use for the factor analysis
totalAll=nrow(qAll) # including those who clicked in lines in the factor question


#matrix-example
table(q[,c('app_install2','F_Ansteckungsrisiko2')])



################################################################################################


#create table of app features that lead to participants changing their mind to the negative
allFactorLoss = data.frame(allFactors)
t=nrow(qYes)+nrow(qUn)
for(i in 1:length(allFactors)){
  sum = sum(polchangeMS2(q,c("app_install",allFactors[i]))$change=="negative")
  allFactorLoss[i,"loss"]=round(100*sum/t, digits = 0)
  #print(paste(i,sum))
}
allFactorLoss <- allFactorLoss[order(-allFactorLoss$loss),]
allFactorLoss


#create table of app features that lead to participants changing their mind to the positve
allFactorWin = data.frame(allFactors)
t=nrow(qYes)+nrow(qUn)
for(i in 1:length(allFactors)){
  sum = sum(polchangeMS2(q,c("app_install",allFactors[i]))$change=="positive")
  allFactorWin[i,"win"]=round(100*sum/t, digits = 0)
  #print(paste(i,sum))
}
allFactorWin <- allFactorWin[order(-allFactorWin$win),]
allFactorWin


#does worry about health affect willingness to use?
table(qAllYes$worry_health)
table(qAllNo$worry_health)
yesWorryHealh=as.numeric(qAllYes$worry_health)
noWorryHealh=as.numeric(qAllNo$worry_health)
summary(yesWorryHealh)
summary(noWorryHealh)
wilcox.test(yesWorryHealh, noWorryHealh, paired = FALSE)

#does worry about the economy affect willingness to use?
table(qAllYes$worry_economy)
table(qAllNo$worry_economy)
yesWorryEconomcy=as.numeric(qAllYes$worry_economy)
noWorryEconomcy=as.numeric(qAllNo$worry_economy)
summary(yesWorryEconomcy)
summary(noWorryEconomcy)
wilcox.test(yesWorryEconomcy, noWorryEconomcy, paired = FALSE)


#does worry about social affect willingness to use?
table(qAllYes$worry_social)
table(qAllNo$worry_social)
yesWorrySocial=as.numeric(qAllYes$worry_social)
noWorrySocial=as.numeric(qAllNo$worry_social)
summary(yesWorrySocial)
summary(noWorrySocial)
wilcox.test(yesWorrySocial, noWorrySocial, paired = FALSE)


riskGroupTabs = xtabs( ~ risk_own + app_install, data=qAll)
riskGroupTabs # show counts
chisq.test(riskGroupTabs)

table(qAllYes$app_install)
risk_install=installYes(qAll[qAll$risk_own=="Ja",]$app_install)
noRisk_install=installYes(qAll[qAll$risk_own=="Nein",]$app_install)
#installYes(qAllYes[qAllYes$risk_own=="Weiß ich nicht",]$app_install)
#installYes(qAllYes[qAllYes$risk_own=="Möchte ich nicht sagen",]$app_install)
risk_noInstall=installNo(qAll[qAll$risk_own=="Ja",]$app_install)
noRisk_noInstall=installNo(qAll[qAll$risk_own=="Nein",]$app_install)



installRiskMatrix = matrix(c(risk_install, noRisk_install, risk_noInstall, noRisk_noInstall),
       nrow = 2,
       dimnames = list( Own_Risk= c("Yes", "No"),
                        Install = c("Yes", "No")))
installRiskMatrix
fisher.test(installRiskMatrix)


# here be dragons -  do not use yet
########### Hypothesen / Tests

## Zentral vs Dezentral
q$F_RKI_Sehen_Andere2 #vs 
RKI_Andere = as.numeric(qYes$F_RKI_Sehen_Andere2)
Andere = as.numeric(qYes$F_Andere2)
mean(RKI_Andere,na.rm = T)
mean(Andere,na.rm = T)
wilcox.test(RKI_Andere, Andere, paired = TRUE)

table(qYes$F_RKI_Sehen_Andere2)
table(qYes$F_Andere2)

#yes(qYes$F_Andere2,qYes$F_RKI_Sehen_Andere2)
#no(qYes$F_Andere2,qYes$F_RKI_Sehen_Andere2)
#usage(qYes$F_Andere2,qYes$F_RKI_Sehen_Andere2)



qYesWay$F_RKI_Sehen_Andere #vs 
as.numeric(qYesWay$F_RKI_Sehen_Andere)

q$F_Andere 

q$F_RKI_Sehen_Ansteckungsrisiko #vs 
q$F_Ansteckungsrisiko 

##Zentral
q$F_Mindestabstand
q$F_Datenbank_Infizierten

##Dezentral
q$F_Hacker




#write_sav(qsAll,"~/sciebo/USECAP/Studies/2020/COVID-App/Qualtrics/Qualtrics2-spss-all.sav")
#write_sav(qs,"~/sciebo/USECAP/Studies/2020/COVID-App/Qualtrics/Qualtrics2-spss-factor.sav")

#quota check
#####README: es ist für Q gemacht. qAll erscheint noch sinvoll. 
n = 1025/100

q$age#in bins einteilen
ageTemp = q$age
ageTemp[is.na(ageTemp)] <- 0
ageInBins = matrix(c(length(ageTemp[ageTemp >= 18 & ageTemp <= 24]), length(ageTemp[ageTemp >= 25 & ageTemp <= 34]), length(ageTemp[ageTemp >= 35 & ageTemp <= 49]), length(ageTemp[ageTemp >= 50 & ageTemp <= 64]), length(ageTemp[ageTemp >= 65]), length(ageTemp[ageTemp == 0])), nrow=6, byrow=TRUE)
ageInBins = t(ageInBins)
colnames(ageInBins) = c("18-24", "25-34", "35-49", "50-64", "65+", "na")
ageQuota <- matrix(c(9.199,15.331,23.915,26.428,25.127,0), ncol=6, byrow=TRUE)
colnames(ageQuota) <- c("18-24", "25-34", "35-49", "50-64", "65+", "na")
a = rbind(ageInBins, ageQuota*n)
barplot(a, beside=T)

eduQuota <- matrix(c(16.473,58.141,25.385), ncol=3, byrow=TRUE)
colnames(eduQuota) = c("ISCED 0-2", "ISCED 3-4", "ISCED 5-8")
eduTemp = table(q$Q91)
eduInBins = matrix(c(eduTemp["Keinen beruflichen Ausbildungsabschluss"], eduTemp["Abgeschlossene Lehre"]+eduTemp["Abitur"],eduTemp["Berufsfachschulabschluss / Fachschulabschluss"]+eduTemp["Meister-, Techniker- oder gleichwertiger Fachschulabschluss"]+eduTemp["Fachhochschulabschluss (auch Abschluss einer Ingenieurschule)"]+ eduTemp["Hochschulabschluss"]),ncol=3, byrow=TRUE)
#freitextantworten coden
colnames(eduInBins) = c("ISCED 0-2", "ISCED 3-4", "ISCED 5-8")
a = rbind(eduInBins, eduQuota*n)
barplot(a, beside=T)

q$income
incomeQuota <- matrix(c(13,5,17,15,14,17,19),ncol=7,byrow=TRUE)
colnames(incomeQuota) <- c("bis 1300","1300 - 1700€","1700 - 2600€","2600 - 3600€","3600 - 5000€","5000-18000€","Möchte ich nicht sagen")
a = rbind(table(q$income), incomeQuota*n)
barplot(a, beside=T)

q$region
regionQuota <- matrix(c(13.125,15.589,4.304,3.067,0.822,2.177,7.486,1.999,9.592,21.644,4.941,1.238,5.044,2.806,3.475,2.693,0), ncol=17, byrow=TRUE)
colnames(regionQuota) <- c("Baden-Württemberg","Bayern","Berlin","Brandenburg","Bremen","Hamburg","Hessen","Mecklenburg-Vorpommern","Möchte ich nicht sagen","Niedersachsen","Nordrhein-Westfalen","Rheinland-Pfalz","Saarland","Sachsen","Sachsen-Anhalt","Schleswig-Holstein","Thüringen")
a = rbind(table(q$region), regionQuota*n)
barplot(a, beside=T)








