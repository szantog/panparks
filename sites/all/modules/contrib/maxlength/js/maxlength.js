(function($) {

  // Make evrything local
  var ml = ml || {};
  ml.options = ml.options || {};

  Drupal.behaviors.maxlength = {
    attach: function(context, settings) {

      if (Drupal.wysiwyg != undefined) {
        $.each(Drupal.wysiwyg.editor.init, function(editor) {
          if (typeof ml[editor] == 'function') {
            ml[editor]();
          }
        });
      }

      $('.maxlength', context).once('maxlength', function() {
        var options = {};
        options['counterText'] = $(this).attr('maxlength_js_label');
        $(this).charCount(options);
      });
    },
    detach: function(context, settings) {
      $('.maxlength', context).removeOnce('maxlength', function() {
        $(this).charCount({
          action: 'detach'
        });
      });
    }
  };

  /**
   * Code below is based on:
   *   Character Count Plugin - jQuery plugin
   *   Dynamic character count for text areas and input fields
   *   written by Alen Grakalic
   *   http://cssglobe.com/post/7161/jquery-plugin-simplest-twitterlike-dynamic-character-count-for-textareas
   *
   *  @param obj
   *    a jQuery object for input elements
   *  @param options
   *    an array of options.
   *  @param count
   *    In case obj.val() wouldn't return the text to count, this should
   *    be passed with the number of characters.
   */
  ml.calculate = function(obj, options, count) {
    var counter = $('#' + obj.attr('id') + '-' + options.css);
    var limit = parseInt(obj.attr('maxlength'));

    if (count == undefined) {
      count = ml.strlen(obj.val());
    }

    var available = limit - count;

    if (available <= options.warning) {
      counter.addClass(options.cssWarning);
    }
    else {
      counter.removeClass(options.cssWarning);
    }

    if (available < 0) {
      counter.addClass(options.cssExceeded);
      // Trim text
      if (options.enforce === true) {
        obj.val(obj.val().substr(0, limit));
        // Re calculate text length
        count = ml.strlen(obj.val());
      }
      available = limit - count;
    }
    else {
      counter.removeClass(options.cssExceeded);
    }

    counter.html(Drupal.t(options.counterText, {'@limit': limit, '@remaining': available}));
  };

  /**
   * Calculate the length of a string using PHP-calculation to count new lines
   * as two characters.
   *
   * @see http://www.sitepoint.com/blogs/2004/02/16/line-endings-in-javascript/
   */
  ml.strlen = function(str) {
    return str.replace(/(\r\n|\r|\n)/g, "\r\n").length;
  };

  $.fn.charCount = function(options) {
    // default configuration properties
    var defaults = {
      warning: 25,
      css: 'counter',
      counterElement: 'div',
      cssWarning: 'messages warning',
      cssExceeded: 'error',
      counterText: 'Characters left: @remaining',
      action: 'attach',
      enforce: false
    };

    var options = $.extend(defaults, options);
    ml.options[$(this).attr('id')] = options;

    if (options.action == 'detach') {
      $(this).removeClass('maxlength-processed');
      $('#' + $(this).attr('id') + '-' + options.css).remove();
      delete ml.options[$(this).attr('id')];
      return 'removed';
    }

    $(this).after('<' + options.counterElement + ' id="' + $(this).attr('id') + '-' + options.css + '" class="' + options.css + '"></' + options.counterElement + '>');
    ml.calculate($(this), options);
    $(this).keyup(function() {
      ml.calculate($(this), options);
    });
    $(this).change(function() {
      ml.calculate($(this), options);
    });

  };

  /**
   * Integrate with WYSIWYG
   * Detect changes on editors and invoke ml.calculate()
   */
  ml.tinymce = function() {
    // We only run it once
    var onlyOnce = false;
    if (!onlyOnce) {
      onlyOnce = true;
      tinyMCE.onAddEditor.add(function(mgr,ed) {
          // the editor is on a maxlength field
          if ($('#' + ed.editorId + '.maxlength').length == 1) {
            ed.onChange.add(function(ed, l) {
              ml.tinymceChange(ed);
            });
            ed.onKeyUp.add(function(ed, e) {
              ml.tinymceChange(ed);
            });
            ed.onPaste.add(function(ed, e) {
              setTimeout(function(){ml.tinymceChange(ed)}, 500);
            });
          }
      });
    }
  }
  // Invoke ml.calculate() for editor
  ml.tinymceChange = function(ed) {
    // CLone to avoid changing defaults
    var options = $.extend({}, ml.options[ed.editorId]);
    // We can't enforce with WYSIWYG
    // HTML makes it hard to control withouth breaking it
    options.enforce = false;
    ml.calculate($(ed.getElement()), options, ml.strlen(ed.getContent()));
  };

  /**
   * Integrate with ckEditor
   * Detect changes on editors and invoke ml.calculate()
   */
  ml.ckeditor = function() {
    // We only run it once
    var onlyOnce = false;
    if (!onlyOnce) {
      onlyOnce = true;
      CKEDITOR.on('instanceReady', function(e) {
        if ($('#' + e.editor.name + '.maxlength').length == 1) {
          ml.options[e.editor.element.getId()].enforce == false;
          e.editor.on('key', function(e) {
            setTimeout(function(){ml.ckeditorChange(e)}, 500);
          });
          e.editor.on('paste', function(e) {
            setTimeout(function(){ml.ckeditorChange(e)}, 500);
          });
        }
      });

    }
  }
  // Invoke ml.calculate() for editor
  ml.ckeditorChange = function(e) {
    // CLone to avoid changing defaults
    var options = $.extend({}, ml.options[e.editor.element.getId()]);
    // We can't enforce with WYSIWYG
    // HTML makes it hard to control withouth breaking it
    options.enforce = false;
    ml.calculate($('#' + e.editor.element.getId()), options, ml.strlen(e.editor.getData()));
  };

})(jQuery);