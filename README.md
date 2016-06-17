#Ork
[Ork home page](http://ork.gforge.inria.fr/)

##Introduction
Ork, for OpenGL rendering kernel, provides a C++ API on top of OpenGL, which greatly simplifies the development of 3D applications.

##Example 
Suppose that you want to draw a mesh in an offscreen framebuffer, with a program that uses a texture. Assuming that these objects are already created, with the OpenGL API you need something like this:
```C++
    glUseProgram(myProgram);
    glActiveTexture(GL_TEXTURE0 + myUnit);
    glBindTexture(GL_TEXTURE_2D, myTexture);
    glUniform1i(glGetUniformLocation(myProgram, "mySampler"), myUnit);
    glBindBuffer(GL_ARRAY_BUFFER, myVBO);
    glVertexAttribPointer(0, 4, GL_FLOAT, false, 16, 0);
    glEnableVertexAttribArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, myFramebuffer);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
```
With the Ork API you simply need two steps (and the first one does not need to be repeated before each draw, unless you want a different texture for each draw):
```C++
    myProgram->getUniformSampler("mySampler")->set(myTexture);
    myFramebuffer->draw(myProgram, *myMesh);
```

## My contribution
I am playing with integrating Proland in a project, and Proland runs on Ork. Since Ork does not allways build and run out of the box, the are some modifications to be done.

