---
layout: post
title: JPSeditor Roadmap
permalink: 2018-09-01-roadmap.html
published: true
date: '2018-09-02'
---

# JPSeditor Roadmap

Welcome, JPSeditor users!

About Four years after the first commit, JPSeditor has grown up to a massive tool, and can  help researchers und students to draw various geometries for pedestrian simulations. But now it's (version 0.8.3) far away from a prefect drawing tool, meanwhile the User Experience of JPSeditor should be improved, in order to make it more user friendly. 

 Therefore there's a lot of ideas and plans about JPSeditor to share. Let’s dive right in!
 
 ## Developing strategy
 
 Right now in the [JPSeditor repository](https://github.com/JuPedSim/jpseditor) there are two branches: master and Beta. The branch "master" is always ready to release new versions, and a bunch of new features have been implemented in the branch "Beta".
 
 Because of lacking of User survey and reports from the focus group, it's difficult to catch the requirements of users. In the later developing, the two branches will **take different Responsibility**.
 
 As a stable branch, the major developing tasks in "master" are **improving User Experience**. It means not only removing bugs，enhancing stability,but also optimizing the operational logic, reorganizing the UI. For example, the operation paths to define rooms and stairs should be more natural.
 
 Unlike with others git flows, the "Beta" branch in JPSeditor project isn't a branch to develop from master (A new branch "dev" will be created from master), rather than a branch to test new features and just for developers. **All new ideas will be implemented and tested in "Beta"**. So at the End the "Beta" won't merged in "master", but the features do.
 
 ## Releasing rhythm
 
 We always want to provide a stable version for users, who will use JPSeditor for studying and experiments. Stability is the life for Them. At the same time we want collecting the issues and bugs from users directly. So the releasing rhythm of JPSeditor will be:
 
 * At 1st of every month, the stable version will be released from "master" as a installation package. The version number will be 0.8.x or 0.9.x;
 * Every 3 months, a beta version version will be released from "Beta" for seed users. The version number will be 0.9 beta or 1.0 beta.
 
 With this releasing strategy, we hope that JPSeditor can serve two types of user in the meantime.
 
 ## one more bad news
 
 Before four years, the goal of JPSeditor project is building a tool to draw geometry for [JPScore](http://www.jupedsim.org/jpscore/), and now, JPSeditor almost became the tool what we imagined. But unfortunately, we won't release JPSeditor version 1.0.
 
 The reason why we won't release version 1.0, let's talk about it in the next post.
