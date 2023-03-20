psa <- read.csv("~/sciebo/USECAP/Teaching/SS20/USECAP Lecture/Exercises/E1 Programming Self Assessment/data/USECAP20 PSA Char R.csv", comment.char="#")
# "./USECAP20 PSA Char R.csv""

#remove non-consenters
psa = psa[psa$We.invite.you.to.take.part.in.the.introductory.study.of.the.Usable.Security.lecture..The.purpose.of.this.questionnaire.is.to.gather.your.self.assessment.of.your.programming.skills.and.some.demographic.data.......There.are.no..right..or..wrong..answers.and.we.will.not.link.the.data.to.you.and.we.will.anonymise.data.before.sharing.it..Participation.in.the.study.is.voluntary.and.you.do.not.need.to.answer.any.questions.you.do.not.feel.comfortable.with..We.estimate.that.it.should.take.around.20.minutes.to.take.part..........The.study.is.lead.by.Stephan.Plöger.and.Matthew.Smith..If.you.have.any.questions.or.concerns.don.t.hesitate.to.contact.us..........I.have.read..understood.and.signed.the.informed.consent.form.handed.out.during.the.lecture.and.I.consent.to.taking.part.in.this.study. =="I consent",]

#remove participants who failed attention check question
psa = psa[psa$What.does.the.abbreviation.CPU.stand.for..This.is.an.attention.check.question..Please.select.Central.Processing.Unifier.as.your.answer.to.show.that.you.have.read.the.question.carefully.=="Central Processing Unifier",]


#Question 2a
xp=psa$How.many.years.of.programming.experience.do.you.have.

hist(xp)
boxplot(xp)
mean(xp)
median(xp)





#Question 2b
os = psa$How.many.open.source.projects.have.you.contributed.to.
hist(os)
median(os)
mean(os)

#Question 2c


cmp=psa$Compared.to.your.fellow.students.in.this.class..how.would.you.judge.your.programming.skills.
  
cmp
table(cmp)


better = sum(cmp=="Much better")
better = better+sum(cmp=="Better")
better = better+sum(cmp=="A bit better")
better

lower = sum(cmp=="Much lower")
lower = lower+sum(cmp=="Lower")
lower = lower+sum(cmp=="A bit lower")
lower

barplot(table(cmp), las=2)

#plot is hard to read because of "wrong" ordering

#define order of variable
cmp2 <- factor(cmp, levels = c('Much Lower', 'Lower', 'A bit Lower','About the Same', 'A bit Better', 'Better', 'Much Better'))
barplot(table(cmp2), las=2)

even = sum(cmp=="About the Same")

(better /(better + even + lower))*100


#Question 3a

enjoy = psa$Please.rate.your.agreement.with.the.following.statement..I.really.enjoy.programming.

enjoy = factor(enjoy, levels = c('Disagree Very Strongly','Disagree Strongly','Disagree','Neither Agree nor Disagree','Agree','Agree Strongly','Agree Very Strongly'))

barplot(table(enjoy))

#to take mean and median because variables are not numberss, they are strings
enjoyNum=as.numeric(enjoy)


#Question 3b
mean(enjoyNum)

#Question 3c
median(enjoyNum)

#adv-disadv
#


#Question 4
askHelp = psa$Please.rate.your.agreement.with.the.following.statements....I.often.ask.for.help.when.programming
askHelp
giveHelp = psa$Please.rate.your.agreement.with.the.following.statements....I.often.help.others.when.I.program
giveHelp

askHelp = factor(askHelp, levels = c('Completely Disagree','Mostly Disagree','Slightly Disagree','Neutral','Slightly Agree','Mostly Agree','Completely Agree'))
giveHelp = factor(giveHelp, levels = c('Completely Disagree','Mostly Disagree','Slightly Disagree','Neutral','Slightly Agree','Mostly Agree','Completely Agree'))

barplot(table(askHelp))
barplot(table(giveHelp))


askHelpNum=as.numeric(askHelp)
giveHelpNum=as.numeric(giveHelp)


diff = giveHelpNum - askHelpNum
barplot(table(diff))


















