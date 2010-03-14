scene{
    render-settings{
        prev-vol{
            width:512;
            height:171;
            samples-per-pixel:5;
            surface-integrator{
                type:redshift;
            }
            volume-integrator{
                type:single;
                step-size:600;
                cutoff-distance:4000;
            }
        }
        prev-no-vol{
            width:1680;
            height:560;
            samples-per-pixel:60;
            surface-integrator{
                type:redshift;
            }
        }
    }
    cameras{
        aerial{
            transform{
                move-up{1700}
                move-right{7000}
                move-backward{900}
                yaw{-90}
                pitch{0}
            }
        }
        hello-world{
            transform{
                move-up{320}
                move-right{2000}
                move-backward{900}
                yaw{-90}
                pitch{0}
            }
        }
        elbow{
            transform{
                move-up{400}
                pitch{45}
                move-backward{1000}
            }
        }
    }
    objects{
        lazy-quadtree{
                color{0.8;0.5;0.3}
                max-recursion:10;
                lod-factor:0.000125;
                size:15000;
                code:
                        (defun foo (x y)
                                (abs ([LayeredNoise2d
                                        filter{cosine}
                                        seed{57}
                                        frequency{0.25}
                                        layercount{9}
                                        persistence{0.54}
                                        levelEvaluationFunction{(abs h)}
                                ] x y))
                        )

                        (defun foo2 (x y)
                                (abs ([LayeredNoise2d
                                        filter{cosine}
                                        seed{57}
                                        frequency{0.25}
                                        layercount{12}
                                        persistence{0.54}
                                        levelEvaluationFunction{(abs h)}
                                ] x y))
                        )

                        (defun bar (x y)
                                (+ (* 0.01
                                      ([LayeredNoise2d layercount{8} filter{cosine} persistence{0.55} frequency{10}] x y)
                                   )
                                   (- 1 (foo
                                        (+ y (^ (foo2 x y) 4))
                                        (+ y (^ (foo2 x y) 4))
                                   ))
                                )
                        )

                        (defun frob (x y) (bar (* 0.00025 x) (* 0.00025 y)))

                        (+ (* 3000  (frob x y))
                           -400)
                ;
        }
        /*horizon-plane{height:300; code:0; color{1;0.5;0.25}}*/
    }
    backgrounds{
        /*preetham-shirley {
                turbidity:2.0;
                sun-direction{2.01;1.0;1.5}
                sky-filter{0.1;0.1;0.1}
                sun-color{7.00;7.00;7.00}
        }*/
    }
    volumes {
        exponential {
                absorption{0.0007;0.0007;0.0007}
                out-scatter{0.001;0.001;0.001}
                emission{0.0006;0.0006;0.0006}
                min{0;300;0}
                phase-function:0;
                base-factor:1;
                exponent-factor:0.005;
        }
    }
}
