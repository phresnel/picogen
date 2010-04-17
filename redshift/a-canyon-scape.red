scene {
        film-settings{
                color-scale:0.019;
                convert-to-srgb:1;
        }

        render-settings{
                prev-vol{
                        width:512;
                        height:512;
                        samples-per-pixel:1;
                        surface-integrator{
                                type:whitted;
                                ambient-samples:1;
                        }
                        volume-integrator{
                                type:none;
                                step-size:40;
                                cutoff-distance:1000000
                        }
                }
                prev-no-vol{
                        width:400;
                        height:300;
                        samples-per-pixel:1;
                        surface-integrator{
                                type:whitted;
                        }
                }
        }
        cameras{
                aerial{
                        front:1.1;
                        transform{
                                move-right{9000}
                                move-up{1450}
                                move-forward{-150000}

                                pitch{-6}
                                roll{0}
                        }
                }
                front{type:cubemap-front; transform{move{9000;1250;-150000}}}
                right{type:cubemap-right; transform{move{9000;1250;-150000}}}
                back{type:cubemap-back; transform{move{9000;1250;-150000}}}
                left{type:cubemap-left; transform{move{9000;1250;-150000}}}
                top{type:cubemap-top; transform{move{9000;1250;-150000}}}
                bottom{type:cubemap-bottom; transform{move{9000;1250;-150000}}}
        }
        objects{
                horizon-plane{
                        material{
                                color{
                                        type:rgb;
                                        rgb{0.5;0.5;0.5}
                                }
                        }
                        height:1015;
                }
                lazy-quadtree{
                        material{
                                color{
                                        type:rgb;
                                        rgb{0.6;0.3;0.15}
                                }
                        }
                        max-recursion:9;
                        lod-factor:0.01;
                        size:1000000;
                        code:   /*(* 1000 (cos (* x 0.001)) (cos (* y 0.001)))*/
                                (defun foo (x y)
                                 (^ (- 1 (abs ([LayeredNoise2d
                                                filter{cosine}
                                                seed{3}
                                                frequency{0.1}
                                                layercount{10}
                                                persistence{0.55}
                                               ] (abs x) (abs y)))) 3)
                                )

                                (defun ridged (x y)
                                 (- 1 (abs (^ (foo x y) 2)))
                                )

                                (+ -2500 (* 9000  (foo (* x 0.0001) (* 0.0001 y))))
                        ;
                }
        }
        backgrounds{
                pss-sunsky {
                        turbidity:3;
                        overcast:0.0;
                        sun-direction{-1.1;0.15;-1}
                        atmospheric-effects:1;
                        sun-brightness-factor:1;
                        atmospheric-effects-distance-factor:1.0;
                }
        }
        volumes {
                homogeneous {
                        absorption{type:rgb; rgb{5.15; 0.15; 0.15}}
                        out-scatter{type:rgb; rgb{0.125;0.125;0.125}}
                        emission{type:rgb; rgb{0.3;0.3;0.3}}
                        min{0;1020;0}
                        phase-function:0;
                        base-factor:1.0;
                        exponent-factor:0.015;
                        epsilon:0.01;
                }
        }
}
